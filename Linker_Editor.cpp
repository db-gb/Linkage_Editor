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

//#include "NumericalStringFunctions.h"
//#include "ListingFileClass.h"
//#include "ListingLineClass.h"
#include "ListingProgramClass.h"

using namespace std;



int main()
{
	string line;
	int lineCnt = 0;

	ListingProgramClass prog;
 

	fstream fileStream;
	fileStream.open("P2sampleWriter.lis");

	prog.AddFileToProgram(&fileStream);
		//ListingFileClass listingFile(&fileStream, 0);

	fileStream.close();

	fileStream.open("P2sampleAdder.lis");

	//ListingFileClass listingFile2(&fileStream, 0);
	prog.AddFileToProgram(&fileStream);
	fileStream.close();

	// Unit Testing
	//UnitTestHexStringToNumConverter();
	//UnitTestDecStringToNumConverter();
	//UnitTestIsEmptyString();
}




