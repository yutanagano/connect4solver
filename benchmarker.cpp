/**
	benchmarker.cpp
	Purpose:	to validate and benchmark connect4 solvers that follow the
				same simple streaming interface:
				- standard input: one position per line (see below for position notation)
				- standard output: space separated position, score, number of explored
				  nodes, computation time in microseconds.
	
	Position notation: A string of numbers corresponding to the played columns.
	E.g. 4453:
	| | | | | | | |
	| | | | | | | |
	| | | | | | | |
	| | | | | | | |
	| | | |2| | | |
	| | |2|1|1| | |
		
	Created while following Pascal Pons' tutorial at blog.gamesolver.org/solving-connect-four
	
	@author Yuta Nagano
	@version 0.5.0
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int open_files(int argc, char* argv[], fstream& dataset, fstream& output);
int open_file(char* fname, fstream& stream);
int compare_files(fstream& dataset, fstream& output);

int main(int argc, char* argv[]) {
	// Declare some fstreams to open the relevant files in
	fstream dataset, output;

	// Try to open the dataset and output files, report any errors
	if (open_files(argc, argv, dataset, output)) return 1;
	
	// Go through each line on both files and analyse, report any errors
	if (compare_files(dataset, output)) return 1;

	return 0;
}

int open_files(int argc, char* argv[], fstream& dataset, fstream& output) {
	// Check that user has supplied two paths: first for dataset, second for output
	if (argc != 3) {
		cout << "Error: bad arguments (requires 2)." << endl;
		cout << "Requires path to dataset and path to output." << endl;
		return 1;
	}

	// Attempt to open dataset file, report any errors
	if (open_file(argv[1],dataset)) return 1;
	// Attempt to open output file, report any errors
	if (open_file(argv[2],output)) return 1;

	return 0;
}

int open_file(char* fname, fstream& stream) {
	// Attempt to open the file using the stream
	stream.open(fname,ios::in);

	// Report any errors
	if (!stream.is_open()) {
		cout << "Error: bad path supplied.\n";
		return 1;
	}

	return 0;
}

int compare_files(fstream& dataset, fstream& output) {
	// Declare necessary variables
	int line_num = 1;
	string temp, dataset_token, output_token;
	istringstream dataset_ss, output_ss;

	// Loop through each line in the dataset file
	while (getline(dataset,temp)) {
		// Store each line in a string stream
		dataset_ss = istringstream(temp);

		// Repeat for the corresponding line in the output file
		getline(output,temp);
		output_ss = istringstream(temp);

		// Look at the first token (position), ensure they are similar
		dataset_ss >> dataset_token;
		output_ss >> output_token;
		if (dataset_token != output_token) {
			cout << "Error: mismatched position on line " << line_num << endl;
			return 1;
		}

		// Look at the second token (score), ensure they are similar
		// (accounting for weak/strong solvers)
		dataset_ss >> dataset_token;
		output_ss >> output_token;

		// | If the scores are not identical
		if (dataset_token != output_token) {
			// Perhaps the output is just a weak solver
			int dataset_score = stoi(dataset_token);
			int output_score = stoi(output_token);
			if (dataset_score * output_score <= 0 || abs(output_score) != 1) {
				cout << "Error: mismatched score on line " << line_num << endl;
			}
		}

		line_num++;
	}

	return 0;
}
