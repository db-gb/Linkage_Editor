#include "ListingFileClass.h"


// Constructor for Listing File Class
// Takes a listing file stream and parses it into a listing line vector.
// Once parsed, obtain valus such as the control section length
ListingFileClass::ListingFileClass(fstream* rawListFile)
{
	string line;
	BaseRegister = 0;
	StartAddr = 0;
	CSectLength = 0;
	lineCnt = 0;

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



void ListingFileClass::SetRegisters(ListingLineClass* line)
{
	cout << "Set Registers called\n";

}