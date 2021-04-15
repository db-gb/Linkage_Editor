#pragma once
#include <iostream>
#include <string>
#include "ListingLineClass.h"

using namespace std;


// This class defines a SymbolTab object that is used to store the 
// addresses associated with the labels defined in a listing file. 
// This object can then be used to create and EXTDEF table that can
// be fed into the ESTAB.
class SymbolTab {

public:
	int Loc = 0;
	string Label;

	// Constructor 
	SymbolTab(ListingLineClass* lstLine)
	{
		Label = "";
		int i;
		// During the 1st pass of the linking process, any lines with 
		// labels are fed to the symbol table
		Loc = lstLine->LocNum;

		for (i = 0; i < lstLine->SymbolName.length(); i++)
		{
			if (!iswspace(lstLine->SymbolName[i]))
			{
				Label.push_back(lstLine->SymbolName[i]);
			}
		}
		//Label = lstLine->SymbolName;
	}


};