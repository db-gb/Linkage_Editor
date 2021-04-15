#include "ListingProgramClass.h"

void ListingProgramClass::AddFileToProgram(fstream* rawListingFile, string filename)
{
	ListingFileClass* listingFile = new ListingFileClass(rawListingFile, filename, TotalProgLen);

	listingFiles.push_back(*listingFile);

	TotalProgLen += listingFile->GetCSectLen();
	numListingFiles++;
}




void ListingProgramClass::PopulateESTAB()
{
	int i;
	for (i = 0; i < numListingFiles; i++)
	{
		ESTABline entry;
		entry.CSect = listingFiles[i].GetCSectName();
		entry.Addr = listingFiles[i].GetCSectLoadAddr();
		entry.Length = listingFiles[i].GetCSectLen();
		ESTAB.push_back(entry);
		numESTABEntries++;

		// First check local SYMTAB to see if label is there
		for (auto& elem : listingFiles[i].ExtDef)
		{
			if (elem->Label != listingFiles[i].GetCSectName())
			{
				ESTABline subEntry;
				subEntry.SymName = elem->Label;
				subEntry.Addr = listingFiles[i].GetCSectLoadAddr() + elem->Loc;
				ESTAB.push_back(subEntry);
				numESTABEntries++;
			}
		}
	}
}



void ListingProgramClass::WriteESTABToFile()
{
	int i;
	string space = "  ";
	fstream fileStream;
	fileStream.open("ESTAB.st", ios::out);

	for (i = 0; i < numESTABEntries; i++)
	{
		fileStream << setfill(' ') << left << setw(6) << ESTAB[i].CSect;
		fileStream << space;
		fileStream << left << setw(6) << ESTAB[i].SymName;
		fileStream << space;
		if (ESTAB[i].Addr >= 0)
		{
			fileStream << setfill('0') << right << setw(6) << uppercase << hex << ESTAB[i].Addr;
		}
		else
		{
			fileStream << setfill('0') << right << setw(6);
		}
		fileStream << space;
		if (ESTAB[i].Length >= 0)
		{
			fileStream << right << setw(6) << uppercase << hex << ESTAB[i].Length;
		}
		else
		{
			fileStream << right << setw(6);
		}
		fileStream << endl;

	}

	fileStream.close();

}


void ListingProgramClass::WriteObjectFiles()
{
	int i;
	

	for (i = 0; i < numListingFiles; i++)
	{
		CreateObjectFile(&listingFiles[i]);
	}
}


void ListingProgramClass::CreateObjectFile(ListingFileClass* listingFile)
{
	int i, placeHolder;
	int colsRemaining;
	int lineStartAddr;
	int recordLength;
	fstream fileStream;
	string textRecord;
	bool skipLine;
	fileStream.open(listingFile->FileName + ".obj", ios::out);

	// Header record
	fileStream << 'H' << setw(6) << left << listingFile->GetCSectName();
	fileStream << setw(6) << setfill('0') << right << hex << uppercase << listingFile->GetStartAddr();
	fileStream << setw(6) << setfill('0') << right << hex << uppercase << listingFile->GetCSectLen();
	fileStream << endl;

	// Define record
	if (listingFile->GetExtDefCount() > 0)
	{
		//placeHolder = 0;
		i = 0;
		while (i < listingFile->GetExtDefCount())
		{
			colsRemaining = 73;
			fileStream << 'D';
			colsRemaining -= 1;

			while (colsRemaining >= 12 && i < listingFile->GetExtDefCount())
			{
				fileStream << setw(6) << left << setfill(' ') << listingFile->ExtDef[i]->Label;
				fileStream << setw(6) << right << setfill('0') << hex << uppercase << listingFile->ExtDef[i]->Loc;
				colsRemaining -= 12;
				i++;
				//placeHolder = i;
			}
			fileStream << endl;
		}
	}

	// Refer record
	if (listingFile->GetExtRefCount() > 0)
	{
		i = 0;
		while (i < listingFile->GetExtRefCount())
		{
			colsRemaining = 73;
			fileStream << 'R';
			colsRemaining -= 1;

			while (colsRemaining >= 6 && i < listingFile->GetExtRefCount())
			{
				fileStream << setw(6) << left << setfill(' ') << listingFile->ExtRef[i];
				colsRemaining -= 6;
				i++;
			}
			fileStream << endl;
		}
	}

	i = 0;
	recordLength = 0;
	lineStartAddr = listingFile->GetStartAddr();
	while (i < listingFile->GetNumLines())
	{
		skipLine = false;
		textRecord = "";
		recordLength = 0;
		colsRemaining = 60;
		if (ParseOperandForLabel(listingFile->listingLines[i].Loc) != "")
		{
			lineStartAddr = listingFile->listingLines[i].LocNum;
		}
		else
		{
			skipLine = true;
			i++;
		}

		while (i < listingFile->GetNumLines() && !skipLine && colsRemaining >= listingFile->listingLines[i].MachInstr.length())
		{
			if (listingFile->listingLines[i].MachInstrLen > 0)
			{
				textRecord.append(listingFile->listingLines[i].MachInstr);
				colsRemaining -= listingFile->listingLines[i].MachInstr.length();
				//lineStartAddr += listingFile->listingLines[i].MachInstrLen;
				recordLength += listingFile->listingLines[i].MachInstrLen;
			}

			if (ParseOperandForLabel(listingFile->listingLines[i].Opcode) == "RESW"
				|| ParseOperandForLabel(listingFile->listingLines[i].Opcode) == "RESB")
			{
				skipLine = true;
			}

			i++;
		}


		if (textRecord.length() > 0)
		{
			fileStream << 'T';
			fileStream << setw(6) << right << setfill('0') << hex << uppercase << lineStartAddr;
			fileStream << setw(2) << right << setfill('0') << hex << uppercase << recordLength;
			fileStream << textRecord;
			fileStream << endl;
		}

		//lineStartAddr = recordLength;
	}





	fileStream.close();

}