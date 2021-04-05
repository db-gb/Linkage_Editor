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
#include <fstream>
#include <string>
#include <vector>

#include "ListingLineObject.h"

using namespace std;



int main()
{
	string line;
	string tempLine;
	int lineCnt = 0;

	vector<ListingLineObject> listingFile;

	fstream fileStream;
	fileStream.open("P2sampleAdder.lis");

	while (getline(fileStream, line))
	{
		lineCnt++;

		cout << line << "\n";
		
	
		listingFile.push_back(ListingLineObject(line));
		

	}

	cout << "Listing file line count: " << lineCnt << "\n";

}


// ParseListingLine
// Method used to parse a listing file line into its various components
// based on the SIC/XE Assembler listing file format.
// Input: (string) Line to parse from listing file
// Output: sets member variables of listing line object
void ListingLineObject::ParseListingLine(string line)
{
	line = line.substr(0, line.find('.'));    // Remove any comments (marked by .)
	int lineLen = line.length();    // Get the line length


	// Grab each of the 5 components of a listing file line

	// Location (in hex, 4 cols)
	Loc = GrabFieldFromLine(line, lineLen, LOCSTART, LOCLEN);
	cout << Loc << "\n";

	// Symbol Name (optional, "space" if none, 6 cols)
	SymbolName = GrabFieldFromLine(line, lineLen, SYMNAMEST, SYMNAMELEN);
	cout << SymbolName << "\n";

	// OPCODE/Compiler Directive (note, 1st col reserved for +, 7 cols)
	Opcode = GrabFieldFromLine(line, lineLen, OPCDST, OPCDLEN);
	
	cout << Opcode << "\n";

	// Operand/Value (note, 1st col reserved for # or @ or =, 24 cols)
	Operand = GrabFieldFromLine(line, lineLen, OPERST, OPERLEN);
	cout << Operand << "\n";

	// Assembled Machine Instruction/Datum (8+ cols, up to 20 cols for a datum
	MachInstr = GrabFieldFromLine(line, lineLen, MACHINST, MACHINLEN);
	cout << MachInstr << "\n";

	if (PC)
	{
		if ((tempDisplacement + Loc) < EOF)
			cout << "temp : " << tempDisplacement;
			MemCheck = true;
		PC = false;
	}
	else
		cout << "Memory error";
			exit;
	if (base)
	{
		if ((tempDisplacement + Loc) < EOF)
			MemCheck = true;
		base = false;
	}
		else
			cout << "Memory error";
			exit;
	if (Immediate)
	{
		if (Operand.substr(1, Operand.size()) < EOF)
		{
			MemCheck = true;
			Immediate = false;

		}
		else
			cout << "Memory error";
			exit;
	}
	if (indexed)
	{

		if()
	}
		
}
