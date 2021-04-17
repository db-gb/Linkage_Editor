#include "ListingProgramClass.h"


// Default Constructor
// Initializes all internal program variables to 0
ListingProgramClass::ListingProgramClass()
{
	numListingFiles = 0;
	TotalProgLen = 0;
	numESTABEntries = 0;
	EndRecordLoc = 0;
	EndRecordFile = 0;
}


// Allows user to add a new listing file to the program
// Takes a filename and attempts to open the file and load it into a ListingFile
// object within the ListingProgram
void ListingProgramClass::AddFileToProgram(string filename)
{
	fstream rawListingFile;

	// Attempt to open file.
	rawListingFile.open(filename);
	if (!rawListingFile)
	{
		cout << "ERROR: Unable to open file: " << filename << endl;
		exit(0);
	}

	// parse filename to get rid of suffix
	filename = filename.substr(0, filename.find('.'));

	// Create a new listing file object at the tail end of the program
	ListingFileClass* listingFile = new ListingFileClass(&rawListingFile, filename, TotalProgLen);

	// add listing file to the program's listing files array
	listingFiles.push_back(*listingFile);

	// Update the program length to include new file's length
	TotalProgLen += listingFile->GetCSectLen();

	// If an end record is specified, mark this file as the endRecordFile
	if (listingFile->EndRecordSpecified)
	{ // NOTE: the program will use the end record of the last file loaded with an end record
		EndRecordFile = numListingFiles;
		EndRecordLoc = listingFile->EndRecord;
	}

	// Increment number of listing files
	numListingFiles++;
	
	// Close the filestream
	rawListingFile.close();
}



// Internal function that populates the ESTAB data structure of the program
void ListingProgramClass::PopulateESTAB()
{
	int i, j;
	// Go through each listing file object
	for (i = 0; i < numListingFiles; i++)
	{
		// Create a new entry for the control section
		ESTABline entry;
		entry.CSect = listingFiles[i].GetCSectName();
		entry.Addr = listingFiles[i].GetCSectLoadAddr();
		entry.Length = listingFiles[i].GetCSectLen();

		// add this entry to the ESTAB
		ESTAB.push_back(entry);
		numESTABEntries++;

		// Labels within a control section
		// First check local SYMTAB to see if label is there
		for (j = 0; j < listingFiles[i].ExtDef.size(); j++)
		{
			if (listingFiles[i].ExtDef[j]->Label != listingFiles[i].GetCSectName())  // ignore control sect name label
			{
				// create new entry for the label
				ESTABline subEntry;
				subEntry.SymName = listingFiles[i].ExtDef[j]->Label;
				subEntry.Addr = listingFiles[i].GetCSectLoadAddr() + listingFiles[i].ExtDef[j]->Loc;

				// add entry to ESTAB
				ESTAB.push_back(subEntry);
				numESTABEntries++;
			}
		}
	}
}


// Function that allows user to tell program to output its 
// ESTAB to a file
void ListingProgramClass::WriteESTABToFile()
{
	int i;
	string space = "  ";
	fstream fileStream;
	fileStream.open("ESTAB.st", ios::out);

	// Create the ESTAB for the file; 
	PopulateESTAB();

	// Print each ESTAB entry
	for (i = 0; i < numESTABEntries; i++)
	{
		// Control Section
		fileStream << setfill(' ') << left << setw(6) << ESTAB[i].CSect;
		fileStream << space;
		// Symbol
		fileStream << left << setw(6) << ESTAB[i].SymName;
		fileStream << space;
		// Address
		fileStream << setfill('0') << right << setw(6) << uppercase << hex << ESTAB[i].Addr;
		fileStream << space;
		// Length
		if (ESTAB[i].Length >= 0)		// only print length if defining CSect
		{
			fileStream << right << setw(6) << uppercase << hex << ESTAB[i].Length;
		}
		else
		{
			fileStream << right << setw(6);
		}
		fileStream << endl;

	}

	// Close completed ESTAB file
	fileStream.close();

}

// Write Object Files
// Function that goes through each file in the program and creates object 
// files for them. No input. Output: object file for each listing file
void ListingProgramClass::WriteObjectFiles()
{
	int i;
	bool endRec;
	
	// Loop through each file
	for (i = 0; i < numListingFiles; i++)
	{
		// check to see if the current file contains the end record
		endRec = false;
		if (i == EndRecordFile)
		{
			endRec = true;
		}

		// Create the object file
		CreateObjectFile(&listingFiles[i], endRec);
	}
}

// Function that creates an object file for a given listing file
void ListingProgramClass::CreateObjectFile(ListingFileClass* listingFile, bool isEndRecord)
{
	fstream fileStream;

	// Create new file with obj suffix
	fileStream.open(listingFile->FileName + ".obj", ios::out);

	// Header record
	WriteHeaderRecord(&fileStream, listingFile);

	// Define record
	WriteExtDefRecord(&fileStream, listingFile);

	// Refer record
	WriteExtRefRecord(&fileStream, listingFile);

	// Text Record
	WriteTextRecord(&fileStream, listingFile);

	// Modification records
	WriteModRecord(&fileStream, listingFile);
	
	// End record
	WriteEndRecord(&fileStream, listingFile, isEndRecord);

	// Close new file
	fileStream.close();

}



// Allows user to search ESTAB for a symbol
// Input: (string) symbol name
// Output: Pointer to ESTABline object if found
struct ListingProgramClass::ESTABline* ListingProgramClass::SearchESTAB(string symbol)
{
	int i;
	int CSectLen;
	
	for (i = 0; i < ESTAB.size(); i++)
	{
		// Only want to return records that have blank control section entry
		CSectLen = ParseOperandForLabel(ESTAB[i].CSect).length();

		// if ESTAB symbol name matches searched symbol name, return the match
		if (ESTAB[i].SymName == ParseOperandForLabel(symbol) && CSectLen == 0)
		{
			return &ESTAB[i];
		}
	}

	// no match found
	return NULL;
}



// Helper function to write the header record to the object file
// Takes a pointer to the filestream that's open, and a pointer to the listing file object.
// Prints H followed by address and length of the listing control section
void ListingProgramClass::WriteHeaderRecord(fstream* fileStream, ListingFileClass* listingFile)
{
	// Header record
	*fileStream << 'H' << setw(6) << left << listingFile->GetCSectName();	// Print H
	*fileStream << setfill('0') << right << hex << uppercase;				// Set format
	*fileStream	<< setw(6) << listingFile->GetStartAddr();					// Set spacing and Print Start Addr
	*fileStream << setw(6) << listingFile->GetCSectLen();					// Print Cont Sect Length
	*fileStream << endl;
}

// Helper function to write the define record to the object file
// Takes a pointer to the filestream that's open, and a pointer to the listing file object.
// Prints D followed by address and length of the listing control section
void ListingProgramClass::WriteExtDefRecord(fstream* fileStream, ListingFileClass* listingFile)
{
	int i, colsRemaining, defRecLen;

	defRecLen = REC_LABEL_LEN + REC_LOC_LEN;

	// Check to see if any ExtDef labels to print
	if (listingFile->GetExtDefCount() > 0)
	{
		i = 0;
		while (i < listingFile->GetExtDefCount())
		{
			colsRemaining = RECORD_COLS;     // columns per Define Record line
			*fileStream << 'D';
			colsRemaining -= 1;		 // D takes up first column

			// While still space in record line and there are still records to print
			while (colsRemaining >= defRecLen && i < listingFile->GetExtDefCount())
			{
				*fileStream << setw(REC_LABEL_LEN) << left << setfill(' ');		  // format extref label
				*fileStream	<< listingFile->ExtDef[i]->Label;		  // print label
				*fileStream << setw(REC_LOC_LEN) << right << setfill('0') << hex << uppercase;    // format loc
				*fileStream << listingFile->ExtDef[i]->Loc;			  // print loc
				colsRemaining -= defRecLen;								// each record takes 12 cols
				i++;
			}
			*fileStream << endl;
		}
	}
}


// Helper function to write the define record to the object file
// Takes a pointer to the filestream that's open, and a pointer to the listing file object.
// Prints D followed by address and length of the listing control section
void ListingProgramClass::WriteExtRefRecord(fstream* fileStream, ListingFileClass* listingFile)
{
	int i, colsRemaining; 

	if (listingFile->GetExtRefCount() > 0)
	{
		i = 0;
		while (i < listingFile->GetExtRefCount())
		{
			colsRemaining = RECORD_COLS;			// columns per record
			*fileStream << 'R';
			colsRemaining -= 1;						// R takes up first column

			// Print records as long as space remaining in line, and records left to print
			while (colsRemaining >= REC_LABEL_LEN && i < listingFile->GetExtRefCount())
			{
				*fileStream << setw(REC_LABEL_LEN) << left << setfill(' ');    // set format
				*fileStream	<< listingFile->ExtRef[i];               // print ExtRef record
				colsRemaining -= REC_LABEL_LEN;				// decrement remaining cols
				i++;
			}

			*fileStream << endl;
		}
	}
}


// Helper function to write the text record to the object file
// Takes a pointer to the filestream that's open, and a pointer to the listing file object.
// Prints T followed by the starting address ,record length, and text record
void ListingProgramClass::WriteTextRecord(fstream* fileStream, ListingFileClass* listingFile)
{
	int i, recordLength, lineStartAddr, colsRemaining;
	int lineLoc, numLinesInListFile, machInstrStringLen, machInstrBytesLen;
	bool skipLine;
	string textRecord, lineLocString, opcode;

	// initialize values
	i = 0;
	recordLength = 0;
	lineStartAddr = listingFile->GetStartAddr();
	numLinesInListFile = listingFile->GetNumLines();


	while (i < numLinesInListFile)
	{
		skipLine = false;
		textRecord = "";
		recordLength = 0;			// starting a new record
		colsRemaining = 60;			// Number of cols for text record

		machInstrStringLen = listingFile->listingLines[i].MachInstr.length();    // current instruction length (cols)
		lineLocString = ParseOperandForLabel(listingFile->listingLines[i].Loc);   // Loc of current instruction
		opcode = ParseOperandForLabel(listingFile->listingLines[i].Opcode);

		if (lineLocString != "")
		{
			lineStartAddr = listingFile->listingLines[i].LocNum;
		}
		else
		{   // if line has no loc, skip it
			skipLine = true;
			i++;
		}

		// continue to add to text record until space runs out or skipLine set
		while (i < numLinesInListFile && !skipLine && colsRemaining >= machInstrStringLen)
		{
			// Get each instruction length and opcode
			machInstrStringLen = listingFile->listingLines[i].MachInstr.length();
			machInstrBytesLen = listingFile->listingLines[i].MachInstrLen;
			opcode = ParseOperandForLabel(listingFile->listingLines[i].Opcode);

			if (machInstrBytesLen > 0)
			{
				// If machine instruction exists, add to current text record
				textRecord.append(listingFile->listingLines[i].MachInstr);
				colsRemaining -= machInstrStringLen;
				recordLength += machInstrBytesLen;
			}

			// If encounter RESW or RESB, end current record
			if (opcode == "RESW" ||  opcode == "RESB")
			{
				skipLine = true;
			}

			i++;
		}

		// Print completed text record
		if (textRecord.length() > 0)
		{
			*fileStream << 'T';
			*fileStream << right << setfill('0') << hex << uppercase;  // set format
			*fileStream << setw(6) << lineStartAddr;        // print record start addr
			*fileStream << setw(2) << recordLength;			// print record length
			*fileStream << textRecord;						// print text record
			*fileStream << endl;
		}

	}

}


// Helper function to write a modification record to the object file
// Takes a pointer to the filestream that's open, and a pointer to the listing file object.
// Prints M followed by the address to modify, the length of the modification, and how to
// modify it
void ListingProgramClass::WriteModRecord(fstream* fileStream, ListingFileClass* listingFile)
{
	int i;
	string symbol1, operation, symbol2;
	string modRecord;
	SymbolTab* symbol1SymTabInfo;
	SymbolTab* symbol2SymTabInfo;
	ESTABline* symbol1EstabInfo;
	ESTABline* symbol2EstabInfo;

	// Loop through each line to see if needs modification,
	// and if so, to print the modification record
	for (i = 0; i < listingFile->GetNumLines(); i++)
	{
		// Grab the line level parsed elements of the operand
		// If there are two symbols being added or subtracted, we'll 
		// catch that here and print the mod record for each symbol
		symbol1 = listingFile->listingLines[i].ExpressionEval->symbol1;
		operation = listingFile->listingLines[i].ExpressionEval->operation;
		symbol2 = listingFile->listingLines[i].ExpressionEval->symbol2;

		// Grab Symtab and ESTAB entries for each symbol, if they exist
		symbol1SymTabInfo = listingFile->FindSymbol(symbol1);
		symbol1EstabInfo = SearchESTAB(symbol1);
		symbol2SymTabInfo = listingFile->FindSymbol(symbol2);
		symbol2EstabInfo = SearchESTAB(symbol2);

		// If there are 2 symbols that have an operation between them 
		// or machine instruction is extended, or the symbol is not in symtab
		// we want to print a mod record
		if (((listingFile->listingLines[i].ExpressionEval->NumSymbols > 1 
			&& listingFile->listingLines[i].ExpressionEval->operation != "") 
			|| listingFile->listingLines[i].Extended || !symbol1SymTabInfo)
			&& listingFile->listingLines[i].MachInstrLen != 0)
		{
			// Check if express is local symbol in SYMTAB
			if (symbol1SymTabInfo)
			{ // if symbol is local, then we add the control section address to the mod record
				*fileStream << 'M';
				*fileStream << right << setfill('0') << hex << uppercase;		// set format
				*fileStream << setw(6) << listingFile->listingLines[i].LocNum + 1;   // print mod address
				*fileStream << setw(2) << 5;									// # of half-bytes to mod
				*fileStream << '+';
				*fileStream << listingFile->GetCSectName();						// add CSect Name
				*fileStream << endl;
			}
			else if (symbol1EstabInfo)     // if symbol not in Symtab, check if in ESTAB
			{   
				*fileStream << 'M';
				*fileStream <<  right << setfill('0') << hex << uppercase;		// set format
				*fileStream << setw(6) << listingFile->listingLines[i].LocNum + 1;    // mod address
				*fileStream << setw(2) << 5;									// # of half-bytes to mod
				*fileStream << '+';
				*fileStream << symbol1;											// add symbol name
				*fileStream << endl;
			}
		}
		// If there is a second symbol in the expression that is being added or subtracted
		if (listingFile->listingLines[i].ExpressionEval->NumSymbols > 1 &&
			(listingFile->listingLines[i].ExpressionEval->operation == "+" 
				|| listingFile->listingLines[i].ExpressionEval->operation == "-"))
		{	
			if (symbol2SymTabInfo)		// check if second symbol in SYMTAB
			{
				*fileStream << 'M';
				*fileStream << right << setfill('0') << hex << uppercase;			// set format
				*fileStream << setw(6) << listingFile->listingLines[i].LocNum + 1;		// mod address
				*fileStream << setw(2) << 5;										// # of half-bytes to mod
				*fileStream << listingFile->listingLines[i].ExpressionEval->operation;  // print + or -
				*fileStream << listingFile->GetCSectName();							// add CSect name
				*fileStream << endl;
			}
			else if (symbol2EstabInfo)		// check if second symbol in ESTAB
			{
				*fileStream << 'M';
				*fileStream << right << setfill('0') << hex << uppercase;			// set format
				*fileStream << setw(6) << listingFile->listingLines[i].LocNum + 1;	// mod address
				*fileStream << setw(2) << 5;										// number of half-bytes to mod
				*fileStream << listingFile->listingLines[i].ExpressionEval->operation;	// print + or -
				*fileStream << symbol2;												// print symbol name
				*fileStream << endl;
			}
		}

	}
}

// Helper function to write the end record to the object file
// Takes a pointer to the filestream that's open, and a pointer to the listing file object.
// Prints and E, then if the file contains the end record, prints the address of the program 
// starting address
void ListingProgramClass::WriteEndRecord(fstream* fileStream, ListingFileClass* listingFile, bool isEndRecord)
{
	// End Record
	*fileStream << 'E';			// print E
	if (isEndRecord)
	{
		// print program start address
		*fileStream << setw(6) << right << setfill('0') << hex << uppercase << EndRecordLoc;  
	}
}