#pragma once
#include "ListingFileClass.h"

using namespace std;


class ListingProgramClass
{
private:
	unsigned int TotalProgLen;

	vector<ListingFileClass> listingFiles;
public:
	ListingProgramClass()
	{
		TotalProgLen = 0;
	}

	void AddFileToProgram(fstream* rawListingFile);
};