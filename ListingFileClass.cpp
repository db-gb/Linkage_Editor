#include "ListingFileClass.h"


// Constructor for Listing File Class
// Takes a listing file stream and parses it into a listing line vector.
// Once parsed, obtain valus such as the control section length
ListingFileClass::ListingFileClass(fstream* rawListFile)
{
	string line;
	BaseRegister = 0;
	StartAddr = 0;
	ProgLength = 0;
	lineCnt = 0;

	// Load each line of the input file into the listingLines vector
	while (getline(*rawListFile, line))
	{
		// Count lines
		lineCnt++;		

		// Remove when done
		cout << line << "\n";

		// Add listing line to vector
		listingLines.push_back(ListingLineClass(line));
	}


	// Get the length of the control section by grabbing the last
	// valid Program Counter value
	for (int i = 0; i < lineCnt; i++)
	{
		// Check if line has Loc
		if (!IsEmptyString(listingLines[i].Loc))
		{
			ProgLength = listingLines[i].ProgCounter;
		}
	}
	//ProgLength = listingLines[lineCnt - 1].ProgCounter;

	/* NOTE: The solution above may seem roundabout, as we should just be able to
	         grab the last line's PC. However, the listing files we are given contain
			 typos where the wrong LOC is calculated. Must rely on LOC, even if wrong 
	*/


	// Remove when done
	cout << "Listing file line count: " << dec << lineCnt << "\n";
	cout << "Total Contr Sect len: " << hex << ProgLength << "\n";

}