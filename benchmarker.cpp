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
	@version 1.1.0
*/

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

int run(int argc, char* argv[]);
int test(int argc, char* argv[]);
int open_files(int argc, char* argv[], ifstream& dataset, ifstream& output, bool verbose = true);
int open_file(string fname, ifstream& stream, bool verbose = true);
int compare_files(ifstream& dataset, ifstream& output, float& mean_explored_nodes, float& mean_time_mics, bool verbose = true);

int main(int argc, char* argv[]) {
	//test(argc, argv);
	run(argc, argv);

	return 0;
}

int run(int argc, char* argv[]) {
	// Declare some fstreams to open the relevant files in
	ifstream dataset, output;
	float mean_explored_nodes, mean_time_mics;

	// Try to open the dataset and output files, report any errors
	if (open_files(argc, argv, dataset, output)) return 1;
	
	// Go through each line on both files and analyse, report any errors
	if (compare_files(dataset, output, mean_explored_nodes, mean_time_mics)) return 1;

	// Close fstreams now that we are done reading
	dataset.close();
	output.close();

	return 0;
}

int test(int argc, char* argv[]) {
	// Declare some fstreams to open the relevant files in
	ifstream dataset, output;
	float mean_explored_nodes, mean_time_mics;

	cout << "Beginning tests..." << endl;

	// Open the test dataset file
	if (open_file("benchmarker_test_files/d",dataset)) return 1;

	// First, open the "o_identical" output file which should have no mismatches
	// Ensure no mismatches found, and validate metrics produced
	if (open_file("benchmarker_test_files/o_identical",output)) return 1;
	if (compare_files(dataset,output,mean_explored_nodes,mean_time_mics,false)) {
		cout << "Test failed: found mismatch where there should be none." << endl;
		return 1;
	}
	if (mean_explored_nodes != 58) {
		cout << "Test failed: bad mean explored node count." << endl;
		cout << "Returned " << mean_explored_nodes << " instead of 58." << endl;
		return 1;
	}
	if (mean_time_mics != 313) {
		cout << "Test failed: bad mean time." << endl;
		cout << "Returned " << mean_time_mics << " instead of 313." << endl;
		return 1;
	}

	// Reset buffers
	output.close();
	dataset.clear();
	dataset.seekg(0);

	// Next, open the "o_bad_score" output file which should have a score mismatch
	// Ensure score mismatch found
	if (open_file("benchmarker_test_files/o_bad_score",output)) return 1;
	if (compare_files(dataset,output,mean_explored_nodes,mean_time_mics,false) != 3) {
		cout << "Test failed: score mismatch missed." << endl;
		return 1;
	}

	// Reset buffers
	output.close();
	dataset.clear();
	dataset.seekg(0);

	// Next, open the "o_missing_line" output file which should return a missing line error
	// Ensure missing line identified
	if (open_file("benchmarker_test_files/o_missing_line",output)) return 1;
	if (compare_files(dataset,output,mean_explored_nodes,mean_time_mics,false) != 1) {
		cout << "Test failed: no missing line error." << endl;
		return 1;
	}

	// Reset buffers
	output.close();
	dataset.clear();
	dataset.seekg(0);

	// Next, open the "o_weaksolver" output file which should have no mismatches
	// Ensure no mismatches found
	if (open_file("benchmarker_test_files/o_weaksolver",output)) return 1;
	if (compare_files(dataset,output,mean_explored_nodes,mean_time_mics,false)) {
		cout << "Test failed: benchmarker not accounting for weak solvers" << endl;
		return 1;
	}

	// Reset buffers
	output.close();
	dataset.clear();
	dataset.seekg(0);

	// Finally, open the "o_wrong_position" file which should have a position mismatch
	// Ensure position mismatch found
	if (open_file("benchmarker_test_files/o_wrong_position",output)) return 1;
	if (compare_files(dataset,output,mean_explored_nodes,mean_time_mics,false) != 2) {
		cout << "Test failed: position mismatch missed." << endl;
		return 1;
	}

	// Close buffers
	dataset.close();
	output.close();

	cout << "Test passed!" << endl;

	return 0;
}

int open_files(int argc, char* argv[], ifstream& dataset, ifstream& output, bool verbose /*=true*/) {
	// Check that user has supplied two paths: first for dataset, second for output
	if (argc != 3) {
		if (verbose) {
			cout << "Error: bad arguments (requires 2)." << endl;
			cout << "Please enter path to dataset and path to output." << endl;
		}
		return 1;
	}

	// Attempt to open dataset file, report any errors
	if (open_file(string(argv[1]),dataset,verbose)) return 1;
	// Attempt to open output file, report any errors
	if (open_file(string(argv[2]),output,verbose)) return 1;

	return 0;
}

int open_file(string fname, ifstream& stream, bool verbose /*=true*/) {
	// Attempt to open the file using the stream
	stream.open(fname);

	// Report any errors
	if (!stream.is_open()) {
		if (verbose) {
			cout << "Error: bad path supplied.\n";
		}
		return 1;
	}

	return 0;
}

int compare_files(ifstream& dataset, ifstream& output, float& mean_explored_nodes, float& mean_time_mics, bool verbose /*=true*/) {
	// Declare necessary variables
	unsigned int line_num = 1;
	unsigned long long c_explored_nodes = 0, c_time_mics = 0;
	string temp, dataset_token, output_token;
	istringstream dataset_ss, output_ss;

	// Loop through each line in the dataset file
	while (getline(dataset,temp)) {
		// Store each line in a string stream
		dataset_ss = istringstream(temp);

		// Repeat for the corresponding line in the output file
		// If output file has fewer lines that dataset, report error
		if (!getline(output,temp)) {
			if (verbose) {
				cout << "Error: missing lines in output file " << line_num << endl;
			}
			return 1;
		}
		output_ss = istringstream(temp);

		// Look at the first token (position), ensure they are similar
		dataset_ss >> dataset_token;
		output_ss >> output_token;
		if (dataset_token != output_token) {
			if (verbose) {
				cout << "Error: mismatched position on line " << line_num << endl;
			}
			return 2;
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
				if (verbose) {
					cout << "Error: mismatched score on line " << line_num << endl;
				}
				return 3;
			}
		}

		// Look at the third and fourth tokens on the output file (# explored nodes and
		// computation time in microseconds) and increment the respective counters
		output_ss >> output_token;
		c_explored_nodes += stoi(output_token);
		output_ss >> output_token;
		c_time_mics += stoi(output_token);

		line_num++;
	}

	// Calculate the mean computation time and mean number of positions explored
	mean_explored_nodes = float(c_explored_nodes) / (line_num - 1);
	mean_time_mics = float(c_time_mics) / (line_num - 1);

	if (verbose) {
		cout << "Solver output validated: no mismatches found." << endl;
		cout << "Benchmarking metrics:" << endl;
		cout << "Mean # of nodes explored per position: " << mean_explored_nodes << endl;
		cout << "Mean time for computation per position (us): " << mean_time_mics << endl;
	}

	return 0;
}
