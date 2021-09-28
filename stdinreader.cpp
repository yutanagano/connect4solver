#include <iostream>
#include <string>

// This is a test program to demo reading lines from standard input

int main() {
	// Define a string s to store lines from the standard intput
	std::string s;

	// Continuously read lines from standard input
	while (std::getline(std::cin, s)) {
		// Output the read line by prepending "I got" behind it
		std::cout << "I got: " << s << std::endl;
	}

	// Return the program execution
	return 0;
}
