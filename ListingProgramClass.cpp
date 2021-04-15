#include "ListingProgramClass.h"

void ListingProgramClass::AddFileToProgram(fstream* rawListingFile)
{
	ListingFileClass* listingFile = new ListingFileClass(rawListingFile, TotalProgLen);

	listingFiles.push_back(*listingFile);

	TotalProgLen += listingFile->GetCSectLen();
}