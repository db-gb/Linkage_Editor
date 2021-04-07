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

	//if (Operand[0] == '=')
	//	Literal = true;
	//else
	//	Literal = false;

	if (Opcode[0] == '=')
		Literal = true;
	else
		Literal = false;

	if ((!MachInstr.empty()) && !Extended && !Immediate && !Indirect && !Literal)
	{

		if (MachInstr[2] == '2' || MachInstr[2] == '3' || MachInstr[2] == 'A' || MachInstr[2] == 'B')
		{
			PCRel = true;
			Displacement = ConvertHexStringToNumber(MachInstr.substr(3, 5));
		}
		else
			PCRel = false;

		if (MachInstr[2] == '4' || MachInstr[2] == '5' || MachInstr[2] == 'C' || MachInstr[2] == 'D')
		{
			BaseRel = true;
		    Displacement = ConvertHexStringToNumber(MachInstr.substr(3, 5));
		}
		else
			BaseRel = false;

		if (MachInstr[2] == '8' || MachInstr[2] == 'A' || MachInstr[2] == 'C')
		{
			Indexed = true;
		}
		else
			Indexed = false;
	}

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




// Update Program Counter
// This function calculates what the program counter value should be while the 
// program is executing the current line. For a normal instruction, the number
// of bytes of the machine instruction is added to Loc. For RESB and RESW, we 
// need to add the number of bytes that are being reserved.
void ListingLineClass::CalcProgramCounter()
{
	string resw = " RESW  ";
	string resb = " RESB  ";
	string word = " WORD  ";
	string byte = " BYTE  ";
	unsigned int oper = 0;
	ProgCounter = LocNum;

	// If has machine instruct, add that length to PC
	if (MachInstrLen > 0)
	{
		ProgCounter += MachInstrLen;
	}
	// Reserve as many words as operand specifies (in decimal)
	else if (Opcode.compare(resw) == 0)
	{
		oper = ConvertDecStringToNumber(Operand);
		ProgCounter += (oper * 3);			// 1 word = 3 bytes
	}
	// Reserve as many bytes as operand specifies (in decimal)
	else if (Opcode.compare(resb) == 0)
	{
		oper = ConvertDecStringToNumber(Operand);
		ProgCounter += oper;
	}

	// These should have a machine instr, but the listing files given do not
	else if (Opcode.compare(word) == 0)
	{
		ProgCounter += 3;
	}
	else if (Opcode.compare(byte) == 0)
	{
		ProgCounter += 1;
	}


	/* Every example in the book shows a machine instr associated with WORD and BYTE,
	   however, the listing files given as examples do not, so we manually add to PC */

}







