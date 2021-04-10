#include "ListingLineObject.h"


// GrabFieldFromLine
// Helper method for parsing the listing line in ParseListingLine method.
// Input: (string) line to parse, (int) length of line to parse
//	      (int) starting position of field, (int) field length per specifications
// Output: (string) field parsed from line
string ListingLineObject::GrabFieldFromLine(string line, int lineLen, int startPos, int fieldLen)
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
void ListingLineObject::SetFlags()
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

	if ((!Opcode.empty()))
	{

		if (Opcode[2] = '2', '3', 'A', 'B')
		{
			PC = true;
			
			tempDisplacement = Opcode.substr(3, 5);
			
			
		}
		else
			PC = false;
		if (Opcode[2] == '4', '5', 'C', 'D')
		{
			tempDisplacement = Opcode.substr(3, 5);
			base = true;
		}
		else
			base = false;
		if (Opcode[2] == '8', 'A', 'C')
		{
			
			indexed = true;
		}
		else
			indexed = false;
	}
		
}

string ListingLineObject::MemoryCheck()
{
	if (PC)
		string tempDisplacement
	
	
}
/*
void ListingLineObject::MemoryCheck()
{
	int a[10];
	string hexStr = Opcode.substr(3);
	int OpcodeNum3 = ConvertHexStringToNumber(hexStr);

	for (int i = 0; OpcodeNum3 > 0; i++)
	{
		a[i] = OpcodeNum3 % 2;
		OpcodeNum3 = OpcodeNum3 / 2;
	}

}
	unsigned int ConvertHexStringToNumber(string hexStr)
	{
		unsigned int digit;
		unsigned int numEquivalent = 0;

		// Loop through each character in string
		for (int i = 0; i < hexStr.length(); i++)
		{
			// Check if char is numerical
			if (hexStr[i] >= '0' && hexStr[i] <= '9')
			{
				digit = hexStr[i] - 0x30;     // Based on ASCII table
			}
			// Check if char is upper case
			else if (hexStr[i] >= 'A' && hexStr[i] <= 'F')
			{
				digit = hexStr[i] - 0x37;
			}
			// Check if char is lower case
			else if (hexStr[i] >= 'a' && hexStr[i] <= 'f')
			{
				digit = hexStr[i] - 0x57;
			}
			else  // Error case: exit with message
			{
				cout << "Invalid hex number -- unable to convert!!\n";
				exit(-1);
			}
			// Shift numerical equivalent by one nybble
			numEquivalent = numEquivalent << 4;

			// Add newest digit to numerical equivalent
			numEquivalent += digit;

		}


		return numEquivalent;
	}
	*/