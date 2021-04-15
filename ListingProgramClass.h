#pragma once
#include <fstream>
#include <iostream>
#include <iomanip>
#include "ListingFileClass.h"

using namespace std;


class ListingProgramClass
{
private:
	struct ESTABline
	{
		string CSect = "";
		string SymName = "";
		int Addr = -1;
		int Length = -1;
	};

	int numListingFiles;
	unsigned int TotalProgLen;
	int numESTABEntries;

	vector<ListingFileClass> listingFiles;
	vector<ESTABline> ESTAB;

public:
	ListingProgramClass()
	{
		numListingFiles = 0;
		TotalProgLen = 0;
	}

	void AddFileToProgram(fstream* rawListingFile, string filename);

	void PopulateESTAB();

	void WriteESTABToFile();

	void CreateObjectFile(ListingFileClass* file);

	void WriteObjectFiles();

	

};