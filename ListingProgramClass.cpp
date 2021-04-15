#include "ListingProgramClass.h"

void ListingProgramClass::AddFileToProgram(fstream* rawListingFile)
{
	ListingFileClass* listingFile = new ListingFileClass(rawListingFile, TotalProgLen);

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