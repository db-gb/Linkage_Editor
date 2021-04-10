#pragma once
#include <iostream>
#include <string>
#include "ListingLineClass.h"

using namespace std;



class SymbolTab {



public:
	int Loc = 0;
	string Label = "";

	SymbolTab(ListingLineClass* lstLine)
	{
		Loc = lstLine->LocNum;
		Label = lstLine->Label;
	}
};