#pragma once
#define RECORD_COLS 73
#define REC_LABEL_LEN 6
#define REC_LOC_LEN 6

#include <fstream>
#include <iostream>
#include <iomanip>
#include "ListingFileClass.h"

using namespace std;


class ListingProgramClass
{
private:

	int numListingFiles;
	int TotalProgLen;
	int numESTABEntries;
	int EndRecordLoc;
	int EndRecordFile;

	// Data structure to store all listing files in program
	vector<ListingFileClass> listingFiles;

	void PopulateESTAB();

	// Function used by program to write object file for a given listing file
	void CreateObjectFile(ListingFileClass* file, bool endRec);

	// Object file writer functions
	void WriteHeaderRecord(fstream* fileStream, ListingFileClass* listingFile);
	void WriteExtDefRecord(fstream*, ListingFileClass* listingFile);
	void WriteExtRefRecord(fstream*, ListingFileClass* listingFile);
	void WriteTextRecord(fstream*, ListingFileClass* listingFile);
	void WriteModRecord(fstream*, ListingFileClass* listingFile);
	void WriteEndRecord(fstream*, ListingFileClass* listingFile, bool isEndRecord);

public:
	

	// ESTAB structure that contains all variables for a record
	struct ESTABline
	{
		string CSect = "";
		string SymName = "";
		int Addr = -1;
		int Length = -1;
	};

	vector<ESTABline> ESTAB;

	// Default Contructor: Initialize all program variables to 0
	ListingProgramClass();

	// Allows user to add a new listing file to the program
	void AddFileToProgram(string filename);

	// Allows user to print ESTAB to file
	void WriteESTABToFile();

	// Allows user to search ESTAB for a symbol
	struct ESTABline* SearchESTAB(string symbol);

	// Write Object Files
	// Function that goes through each file in the program and creates object 
	//files for them. No input. Output: object file for each listing file
	void WriteObjectFiles();



};