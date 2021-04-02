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

#include "StringHexFunctions.h"
#include "ListingFileClass.h"
#include "ListingLineClass.h"

using namespace std;



int main()
{
	//ListingFileClass listingFile;
	string line;
	int lineCnt = 0;

	UnitTestHexStringToNumConverter();
 

	fstream fileStream;
	fileStream.open("P2sampleAdder.lis");

	ListingFileClass listingFile(&fileStream);



}




