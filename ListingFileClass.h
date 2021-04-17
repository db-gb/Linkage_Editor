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
	int StartAddr;
	int CSectLength;
	int LoadAddr;
	int BaseRegister;
	int XRegister;
	bool ExtDefExists;
	bool ExtRefExists;
	int StartLine;
	int ExtDefLine;
	int ExtDefCount;
	int ExtRefLine;
	int ExtRefCount;
	int lineCnt;
	int EndLine;

	string CSectName;
	
	
	vector<SymbolTab> SymTab;
	
	

	void SetRegisters(ListingLineClass*);

public:
	string FileName;
	vector<SymbolTab*> ExtDef;
	vector<string> ExtRef;
	vector<ListingLineClass> listingLines;
	bool EndRecordSpecified;
	int EndRecord;

	ListingFileClass(fstream* rawListFile, string filename, int loadAddr);

	void PopulateExtDef(string extDefLabels);

	void PopulateExtRef(string);

	SymbolTab* FindSymbol(string label);

	int FindSymbolLoc(string label);

	bool PerformInternalMemoryCheck();


	string GetCSectName()
	{
		return CSectName;
	}

	int GetCSectLen()
	{
		return CSectLength;
	}

	int GetCSectLoadAddr()
	{
		return LoadAddr;
	}
	
	int GetStartAddr()
	{
		return StartAddr;
	}

	int GetExtDefCount()
	{
		return ExtDefCount;
	}

	int GetExtRefCount()
	{
		return ExtRefCount;
	}

	int GetNumLines()
	{
		return lineCnt;
	}

};
