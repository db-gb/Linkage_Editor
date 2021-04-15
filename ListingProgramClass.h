#pragma once
#include "ListingFileClass.h"

using namespace std;


class ListingProgramClass
{
private:
	struct ESTABline
	{
		string CSect = "";
		string SymName = "";
		unsigned int Addr = -1;
		unsigned int Length = -1;
	};

	int numListingFiles;
	unsigned int TotalProgLen;

	vector<ListingFileClass> listingFiles;
	vector<ESTABline> ESTAB;

public:
	ListingProgramClass()
	{
		numListingFiles = 0;
		TotalProgLen = 0;
	}

	void AddFileToProgram(fstream* rawListingFile);

	void PopulateESTAB();

	

};