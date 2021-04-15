#include "ListingFileClass.h"


// Constructor for Listing File Class
// Takes a listing file stream and parses it into a listing line vector.
// Once parsed, obtain valus such as the control section length
ListingFileClass::ListingFileClass(fstream* rawListFile, string filename, int loadAddr)
{
	FileName = filename;
	string line;
	BaseRegister = 0;
	StartAddr = 0;
	CSectLength = 0;
	lineCnt = 0;
	ExtDefCount = 0;
	ExtRefCount = 0;
	ExtDefExists = false;
	ExtRefExists = false;
	LoadAddr = loadAddr;

	// Load each line of the input file into the listingLines vector
	while (getline(*rawListFile, line))
	{
		// Count lines
		lineCnt++;

		// Remove when done
		cout << line << "\n";

		// Add listing line to vector
		listingLines.push_back(ListingLineClass(line));
	}


	// Get the length of the control section by grabbing the last
	// valid Program Counter value
	for (int i = 0; i < lineCnt; i++)
	{
		// Check if line has Loc
		if (!IsEmptyString(listingLines[i].Loc))
		{
			CSectLength = listingLines[i].ProgCounter;
		}
		if (!IsEmptyString(listingLines[i].SymbolName))
		{
			SymTab.push_back(SymbolTab(&listingLines[i]));
		}
		if (listingLines[i].Opcode == " EXTDEF")
		{
			ExtDefExists = true;
			ExtDefLine = i;
		}
		if (listingLines[i].Opcode == " EXTREF")
		{
			ExtRefExists = true;
			ExtRefLine = i;
		}
		if (ParseOperandForLabel(listingLines[i].Opcode) == "START")
		{
			StartLine = i;
		}
	}

	CSectName = ParseOperandForLabel(listingLines[StartLine].SymbolName);

	// Populate EXTDEF and EXTREF data structures
	if (ExtRefExists)
	{
		PopulateExtRef(listingLines[ExtRefLine].Operand);
	}
	if (ExtDefExists)
	{
		PopulateExtDef(listingLines[ExtDefLine].Operand);
	}


	//ProgLength = listingLines[lineCnt - 1].ProgCounter;

	/* NOTE: The solution above may seem roundabout, as we should just be able to
			 grab the last line's PC. However, the listing files we are given contain
			 typos where the wrong LOC is calculated. Must rely on LOC, even if wrong
	*/




	// Remove when done
	cout << "Listing file line count: " << dec << lineCnt << "\n";
	cout << "Total Contr Sect len: " << hex << CSectLength << "\n";

	PerformInternalMemoryCheck();

}



// Internal Memory Check
// Once the listing file has been parsed and loaded into a ListingFileClass object, this
// method checks to see that all the relative instructions within the control section are
// valid (i.e. they do not exceed the control section's length or go below 0)
// Returns 1 if no illegal memory access attempts, 0 if test fails
bool ListingFileClass::PerformInternalMemoryCheck()
{
	// Start with a TA of 0
	unsigned int targetAddr = 0;

	for (int i = 0; i < lineCnt; i++)
	{
		// If the current instruction changes the value of BASE or the X register
		// we want to catch that here
		SetRegisters(&listingLines[i]);

		// We only want to deal with Format 3 instructions
		if (listingLines[i].MachInstrLen == 3)
		{
			// PC Relative displacements
			if (listingLines[i].PCRel)
			{
				targetAddr = listingLines[i].ProgCounter + listingLines[i].Displacement;
			}
			// Base Relative displacements
			else if (listingLines[i].BaseRel)
			{
				targetAddr = BaseRegister + listingLines[i].Displacement;
			}

			// Indexed 
			if (listingLines[i].Indexed)
			{
				targetAddr += XRegister;
			}

			// Check if TA within bounds
			if (targetAddr < 0 || targetAddr >= CSectLength)
			{
				cout << "ERROR: Attempt to reach illegal memory location at LOC: " << listingLines[i].Loc << "\n";
				return 0;
			}

		}
	}

	return 1;
}

// Search SymTab for loc
// This function takes a label and checks to see if the symtab has an
// entry corresponding to the label. If so, returns the loc. 
// If not, check EXTDEF table to see if label defined externally.
// If not found there, issue error message and exit the program
int ListingFileClass::FindSymbolLoc(string label)
{
	SymbolTab* symtab;

	// First check local SYMTAB to see if label is there
	symtab = FindSymbol(label);

	if (symtab)
	{
		// if so, return the loc
		return symtab->Loc;
	}
	else
	{
		// if symbol not found, might be in other file; return 0 for now
		return 0;
	}
}


// Search SymTab for loc
// This function takes a label and checks to see if the symtab has an
// entry corresponding to the label. If so, returns the loc. 
// If not, check EXTDEF table to see if label defined externally.
// If not found there, issue error message and exit the program
SymbolTab* ListingFileClass::FindSymbol(string label)
{
	// First check local SYMTAB to see if label is there
	for (auto& elem : SymTab)
	{
		if (elem.Label == label)
		{
			// if so, return the loc
			return &elem;
		}
	}

	// if symbol not found, might be in other file; return 0 for now
	return NULL;


}


// Populate ExtDef
// When loading the file, if we come across a line that has an OPCODE 
// of EXTDEF, we want to populate the ExtDef array with the Label name, 
// and the corresponding loc. This will come in handy when we want to 
// create our ESTAB
void ListingFileClass::PopulateExtDef(string extDefLabels)
{
	string lbl = "";
	int i = 0;
	SymbolTab *symtab;

	// Want to traverse entire EXTDEF labels list
	for (i = 0; i < extDefLabels.length(); i++)
	{
		// capture any chars between spaces or commas
		if (!iswspace(extDefLabels[i]) && extDefLabels[i] != ',')
		{
			lbl.push_back(extDefLabels[i]);
		}
		// if space, comma, or last character, consider label complete
		// and search for it in SYMTAB
		if (iswspace(extDefLabels[i]) 
			|| extDefLabels[i] == ',' 
				||	i == (extDefLabels.length() - 1))
		{
			if (lbl.length() > 0)
			{
				// Check SYMTAB for symbol with the parsed label name
				symtab = FindSymbol(lbl);
				if (symtab)
				{
					// If symbol defined in SYMTAB, add a pointer
					// to EXTDEF
					ExtDef.push_back(symtab);
					ExtDefCount++;
				}
			}
			lbl = "";		// reset the label to empty string
		}

	}


}



void ListingFileClass::PopulateExtRef(string extRefString)
{
	string lbl = "";
	int i = 0;

	// Want to traverse entire EXTREF labels list
	for (i = 0; i < extRefString.length(); i++)
	{
		// capture any chars between spaces or commas
		if (!iswspace(extRefString[i]) && extRefString[i] != ',')
		{
			lbl.push_back(extRefString[i]);
		}
		// if space, comma, or last character, consider label complete
		if (iswspace(extRefString[i])
			|| extRefString[i] == ','
			|| i == (extRefString.length() - 1))
		{
			if (lbl.length() > 0)
			{
				ExtRef.push_back(lbl);
				ExtRefCount++;
			}
			lbl = "";		// reset the label to empty string
		}
	}
}





void ListingFileClass::SetRegisters(ListingLineClass* line)
{
	cout << "Set Registers called\n";

	// Handle setting X register
	if (line->Opcode.find("LDX") != string::npos)
	{
		this->XRegister = ConvertDecStringToNumber(line->Operand);
	}

	if (line->Opcode.find("BASE") != string::npos)
	{
		this->BaseRegister = FindSymbolLoc(ParseOperandForLabel(line->Operand));
	}



}