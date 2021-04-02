#include "ListingFileClass.h"

ListingFileClass::ListingFileClass(fstream* rawListFile)
{
	string line;
	BaseRegister = 0;
	StartAddr = 0;
	ProgLength = 0;

	lineCnt = 0;

	while (getline(*rawListFile, line))
	{
		lineCnt++;

		cout << line << "\n";

		listingLines.push_back(ListingLineClass(line));
	}

	cout << "Listing file line count: " << lineCnt << "\n";


	cout << "Total Contr Sect len: " << ProgLength << "\n";

}