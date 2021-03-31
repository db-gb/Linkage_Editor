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
// Set the addressing mode and instruction flags based on the 
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

}