#include "ListingLineClass.h"




// ParseListingLine
// Method used to parse a listing file line into its various components
// based on the SIC/XE Assembler listing file format.
// Input: (string) Line to parse from listing file
// Output: sets member variables of listing line object
void ListingLineClass::ParseListingLine(string line)
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


}


// GrabFieldFromLine
// Helper method for parsing the listing line in ParseListingLine method.
// Input: (string) line to parse, (int) length of line to parse
//	      (int) starting position of field, (int) field length per specifications
// Output: (string) field parsed from line
string ListingLineClass::GrabFieldFromLine(string line, int lineLen, int startPos, int fieldLen)
{
	// Check if line is long enough to contain the field
	if (lineLen > startPos)
	{
		// Check if line contains all columns specified for the field
		if ((lineLen - startPos) > fieldLen)
		{
			// If so, return entire field per specifications
			return line.substr(startPos, fieldLen);
		}
		else
		{
			// Otherwise, return only the columns contained in line
			return line.substr(startPos, (lineLen - startPos));
		}
	}

	// Line too short, does not contain field, return empty string
	return "";
}


// SetFlags
// Set the addressing mode and instruction flags based on whether the
// original listing line has the corresponding symbol in the appropriate
// column
// Inputs and Outputs: none
// Sets the flags internal to the object
void ListingLineClass::SetFlags()
{

	if (Opcode[0] == '+')
		Extended = true;
	else
		Extended = false;

	if (Operand[0] == '#')
		Immediate = true;
	else
		Immediate = false;

	if (Operand[0] == '@')
		Indirect = true;
	else
		Indirect = false;

	if (Operand[0] == '=')
		Literal = true;
	else
		Literal = false;

}



// CalcMachInstrLen
// Helper function that calculates how many bytes the machine instruction of
// the listing line occupies. This comes in handy for calculating the total
// size of the control section as well as for getting the Program Counter 
// value
void ListingLineClass::CalcMachInstrLen()
{
	//cout << "Machine Instr len: " << MachInstr.length() << "\n";
	MachInstrLen = MachInstr.length() / 2;

}