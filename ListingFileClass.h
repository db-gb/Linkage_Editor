#pragma once


#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "ListingLineClass.h"
#include "SymbolTab.h"

using namespace std;

class ListingFileClass
{
private:
	unsigned int StartAddr;
	unsigned int CSectLength;
	unsigned int BaseRegister;
	unsigned int XRegister;
	int lineCnt;

	vector<SymbolTab> SymTab;
	vector<ListingLineClass> listingLines;

	void SetRegisters(ListingLineClass*);

public:

	ListingFileClass(fstream* rawListFile);

	bool PerformInternalMemoryCheck();

};