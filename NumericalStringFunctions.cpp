#include "NumericalStringFunctions.h"

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

	// Check for negative - if the number is negative, want hex to start at all Fs
	for (int i = 0; i < hexStr.length(); i++)
	{
		if (iswspace(hexStr[i]))
			continue;
		if (hexStr[i] == '8' || hexStr[i] == '9')
		{
			numEquivalent = -1;
			break;
		}
		else if (toupper(hexStr[i]) >= 'A' && toupper(hexStr[i]) <= 'F')
		{
			numEquivalent = -1;
			break;
		}
		else
		{
			break;
		}
	}

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
		// ignore spaces and other whitespaces
		else if (iswspace(hexStr[i]))
		{
			continue;
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


// Convert strings of decimal numbers into their
// numerical equivalents so they can be manipulated by program
// Input: Takes a decimal number string
// Output: Returns the numerical equivalent of that number or 
//         exits if invalid number
unsigned int ConvertDecStringToNumber(string decStr)
{
	unsigned int digit;
	unsigned int numEquivalent = 0;

	// Loop through each character in string
	for (int i = 0; i < decStr.length(); i++)
	{
		// Check if char is numerical
		if (decStr[i] >= '0' && decStr[i] <= '9')
		{
			digit = decStr[i] - 0x30;     // Based on ASCII table
		}
		// ignore spaces and other whitespaces
		else if (iswspace(decStr[i]) || decStr[i] == '#')
		{
			continue;
		}
		else  // Error case: exit with message
		{
			cout << "Invalid hex number -- unable to convert!!\n";
			exit(-1);
		}
		// Shift numerical equivalent by 10 to make room for new digit
		numEquivalent = numEquivalent * 10;

		// Add newest digit to numerical equivalent
		numEquivalent += digit;

	}

	return numEquivalent;
}


// IsEmptyString
// Helper function to check if a string contains only white
// space characters.
// Input: string that you're testing
// Output: bool: true is string contains only white spaces
bool IsEmptyString(string testStr)
{
	// Iterate through every character
	for (int i = 0; i < testStr.length(); i++)
	{
		// If character is not a white space char, test is done
		if (!iswspace(testStr[i]))
		{
			return false;
		}
	}

	// Only white space chars found
	return true;
}

// Helper function to parse and operand for the Label that it refers to.
// Eliminates leading space, and grabs the string up to the first comma, 
// in case the operand is indexed.
string ParseOperandForLabel(string operString)
{
	int i = 0;
	int operStrLen;
	string lbl = "";

	operStrLen = operString.length();

	// Traverse operand until the first comma, or entire length
	while (operString[i] != ',' && i < operStrLen)
	{
		// ignore # and leading space
		if (operString[i] != '#' && !iswspace(operString[i]))
		{
			// add valid character to label
			lbl.push_back(operString[i]);
		}
		i++;
	}

	return lbl;
}



// Unit testing
void UnitTestHexStringToNumConverter()
{
	string testNumStr;
	int testNum = 0;

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




// Unit testing
void UnitTestDecStringToNumConverter()
{
	string testNumStr;
	unsigned int testNum = 0;

	testNumStr = "5";
	testNum = ConvertDecStringToNumber(testNumStr);
	assert(testNum == 5);

	testNumStr = "34";
	testNum = ConvertDecStringToNumber(testNumStr);
	assert(testNum == 34);

	testNumStr = "1000";
	testNum = ConvertDecStringToNumber(testNumStr);
	assert(testNum == 1000);

	testNumStr = " 0 ";
	testNum = ConvertDecStringToNumber(testNumStr);
	assert(testNum == 0);

	testNumStr = " 123 456 ";
	testNum = ConvertDecStringToNumber(testNumStr);
	assert(testNum == 123456);

	testNumStr = "005";
	testNum = ConvertDecStringToNumber(testNumStr);
	assert(testNum == 5);

	cout << "Dec String to Number Converter tests passed!\n";
}



void UnitTestIsEmptyString()
{
	string testStr;
	bool testBool;

	testStr = "5";
	testBool = IsEmptyString(testStr);
	assert(!testBool);

	testStr = "0";
	testBool = IsEmptyString(testStr);
	assert(!testBool);

	testStr = " ";
	testBool = IsEmptyString(testStr);
	assert(testBool);

	testStr = "    7";
	testBool = IsEmptyString(testStr);
	assert(!testBool);

	testStr = "			 ";
	testBool = IsEmptyString(testStr);
	assert(testBool);

	testStr = "	";
	testBool = IsEmptyString(testStr);
	assert(testBool);

	cout << "IsEmptyString tests passed!\n";

}