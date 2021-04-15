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

		// First check local SYMTAB to see if label is there
		for (auto& elem : listingFiles[i].ExtDef)
		{
			if (elem->Label != listingFiles[i].GetCSectName())
			{
				ESTABline subEntry;
				subEntry.SymName = elem->Label;
				subEntry.Addr = listingFiles[i].GetCSectLoadAddr() + elem->Loc;
				ESTAB.push_back(subEntry);
			}
		}

		// if symbol not found, might be in other file; return 0 for now
		//return NULL;
	}


}