#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "ListingLineClass.h"

using namespace std;

class ListingFileClass
{
private:
	unsigned int StartAddr;
	unsigned int ProgLength;
	unsigned int BaseRegister;
	int lineCnt;
	
	vector<ListingLineClass> listingLines;

public:

	ListingFileClass(fstream* rawListFile);

};