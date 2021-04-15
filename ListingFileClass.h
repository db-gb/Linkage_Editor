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
	unsigned int LoadAddr;
	unsigned int BaseRegister;
	unsigned int XRegister;
	bool ExtDefExists;
	bool ExtRefExists;
	int StartLine;
	int ExtDefLine;
	int ExtRefLine;
	int lineCnt;

	string CSectName;
	vector<SymbolTab> SymTab;
	vector<SymbolTab*> ExtDef;
	vector<string> ExtRef;
	vector<ListingLineClass> listingLines;

	void SetRegisters(ListingLineClass*);

public:


	ListingFileClass(fstream* rawListFile, unsigned int loadAddr);

	void PopulateExtDef(string extDefLabels);

	void PopulateExtRef(string);

	SymbolTab* FindSymbol(string label);

	int FindSymbolLoc(string label);

	bool PerformInternalMemoryCheck();


	string GetCSectName()
	{
		return CSectName;
	}

	unsigned int GetCSectLen()
	{
		return CSectLength;
	}

};