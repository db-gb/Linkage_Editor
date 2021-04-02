#include "StringHexFunctions.h"

using namespace std;


// Convert strings of hexadecimals numbers into their
// numerical equivalents so they can be manipulated by program
// Input: Takes a hexadecimal number string
// Output: Returns the numerical equivalent of that number or 
//         exits if invalid number
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




// Unit testing
void UnitTestHexStringToNumConverter()
{
	string testNumStr;
	unsigned int testNum = 0;

	testNumStr = "a";
	testNum = ConvertHexStringToNumber(testNumStr);
	assert(testNum == 10);

	testNumStr = "A";
	testNum = ConvertHexStringToNumber(testNumStr);
	assert(testNum == 10);

	testNumStr = "1000";
	testNum = ConvertHexStringToNumber(testNumStr);
	assert(testNum == 4096);

	testNumStr = "0";
	testNum = ConvertHexStringToNumber(testNumStr);
	assert(testNum == 0);

	testNumStr = "1ab";
	testNum = ConvertHexStringToNumber(testNumStr);
	assert(testNum == 427);

	testNumStr = "12ab56CD";
	testNum = ConvertHexStringToNumber(testNumStr);
	assert(testNum == 313218765);

	cout << "Hex String to Number Converter tests passed!\n";
}