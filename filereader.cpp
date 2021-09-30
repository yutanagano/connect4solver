/**
	filereader.cpp
	Purpose: to practice reading from text files in C++

	@author Yuta Nagano
	@version 1.0.0
*/

/*
	The function of this program in pseudocode:

	Begin
		Create an object newfile against class fstream
		Call open() on newfile to open a file to perform write operation
		If file is open
			Write a practice text into the file
			Close newfile by calling close()
		Call open() on newfile to open the same file to perform read operation
		If file is open
			Declare string s
			Read all data of newfile using getline() and put it in s
			print newfile by calling close()
	End
*/
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main () {
	// Create an object newfile against class fstream
	fstream newfile;

	// Open test.txt using newfile by calling open(), with mode out
	newfile.open("test.txt",ios_base::out);

	// Check whether the file has been opened correctly
	if (newfile.is_open()) {
		// Put in test text into the file
		newfile << "This is some test text.\nI wonder if we can read multiple lines without any problems\n";
		// Close the file
		newfile.close();
	} else {
		cout << "The file could not be opened for writing.\n";
		return 1;
	}

	// Open test.txt again using newfile using open(), with mode in
	newfile.open("test.txt",ios_base::in);

	// Check whether the file has been opened correctly
	if (newfile.is_open()) {
		// Declare a string s
		string s;
		// Loop through each line of the file and put the contents into s
		while (getline(newfile, s)) {
			// Print the line to the standard output
			cout << s << endl;
		}
		// Close the file
		newfile.close();
	} else {
		cout << "The file could not be opened for reading.\n";
		return 1;
	}

	return 0;
}
