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
	int i;
	int lineCnt = 0;
	string filename;

	ListingProgramClass prog;
 

	//fstream fileStream;
	//fileStream.open("P2sampleAdder.lis");
	filename = "P2sampleAdder.lis";
	//ListingFileClass listingFile2(&fileStream, 0);
	prog.AddFileToProgram(/*&fileStream,*/ filename);
	//fileStream.close();


	//fileStream.open("P2sampleWriter.lis");
	filename = "P2sampleWriter.lis";
	prog.AddFileToProgram(/*&fileStream, */ filename);
		//ListingFileClass listingFile(&fileStream, 0);

	//fileStream.close();



	//prog.PopulateESTAB();
	prog.WriteESTABToFile();
	prog.WriteObjectFiles();

	// Unit Testing
	//UnitTestHexStringToNumConverter();
	//UnitTestDecStringToNumConverter();
	//UnitTestIsEmptyString();
}




