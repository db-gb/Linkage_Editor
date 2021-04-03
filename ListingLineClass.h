#pragma once
#define LOCSTART 0
#define LOCLEN 4
#define SYMNAMEST 8
#define SYMNAMELEN 6
#define OPCDST 16
#define OPCDLEN 7
#define OPERST 25
#define OPERLEN 24
#define MACHINST 51
#define MACHINLEN 20

#include <iostream>
#include <string>

#include "NumericalStringFunctions.h"

using namespace std;



class ListingLineClass {
private:


	void ParseListingLine(string line);
	string GrabFieldFromLine(string line, int lineLen, int startPos, int length);
	void CalcMachInstrLen();
	void CalcProgramCounter();
	void SetFlags();

public:
	string Loc;
	string SymbolName;
	string Opcode;
	string Operand;
	string MachInstr;
	bool Extended;
	bool Immediate;
	bool Indirect;
	bool Literal;
	unsigned int ProgCounter;
	unsigned int LocNum;
	int MachInstrLen; 

	ListingLineClass(string line)
	{
		MachInstrLen = 0;
		ProgCounter = 0;
		ParseListingLine(line);
		SetFlags();
		CalcMachInstrLen();
		LocNum = ConvertHexStringToNumber(Loc);
		CalcProgramCounter();


		// Remove when done//////////////////////////////////////
		if (Extended)
			cout << "Extended\n";
		if (Immediate)
			cout << "Immediate\n";
		if (Indirect)
			cout << "Indirect\n";
		if (Literal)
			cout << "Literal\n";

		cout << "PC: " << hex << ProgCounter << "\n";
		//////////////////////////////////////////////////////////

	}



};