/**
 * solver.cpp
 * Purpose: solver program that can evaluate a given connect4 position, given
 * the simple interface as follows:
 * - standard input: one position per line (see below for position notation).
 * - standard output: space separated position, score, number of explored nodes, 
 *   computation time in microseconds.
 *
 * Position notation: a string of numbers corresponding to the played columns.
 * E.g. 4453:
 * | | | | | | | |
 * | | | | | | | |
 * | | | | | | | |
 * | | | | | | | |
 * | | | |2| | | |
 * | | |2|1|1| | |
 *
 * @author: Yuta Nagano
 * @version: 1.0.0
 */

#include <iostream>
#include <string>
#include <stdexcept>
#include <chrono>
#include <algorithm>
#include <cctype>
#include "position/position.hpp"

using namespace std;
using namespace std::chrono;

/**
 * Checks if a given line (string) contains only digit chars.
 * @return 1 if string contains only digits, 0 otherwise
 */
int only_digits(const string& line);

/**
 * Recursively solve a connect4 position using the negamax variant of the
 * minimax algorithm. A reference to an int counter should be passed in, which
 * will be incremented every time a new position is evaluated in the recursive
 * algorithm.
 * @return the score of a position:
 *  - 0 for a draw
 *  - positive score for a forcing win, where the numerical value corresponds to the
 *    number of turns before the maximum possible turns that you win (that is,
 *    the earlier you win, the higher your score)
 *  - negative score for a forcing loss, where the numerical value corresponds
 *    to the number of turns before the maximum possible turns that you lose 
 *    (that is, the earlier you lose, the lower your score)
 */
int negamax(const Position& P, int& position_counter);

/**
 * read every line from the standard input, which should contain an encoding
 * of a connect4 position (read above for the position encoding syntax), evaluate
 * that position, and return that position's score, number of evaluated positions,
 * and computation time in microseconds.
 */
int main() {
	// Declare a string to store the read lines in, a position object to store
	// positions in, and ints for the score and a counter to track the number
	// of positions explored
	string line;
	Position position;
	int score, counter;

	while (getline(cin, line)) {
		if (!only_digits(line))
			throw runtime_error("Input contains lines with non-digit charcters.");
		counter = 0;
		position = Position(line);

		// take a note of the time to measure execution time in microseconds
		high_resolution_clock::time_point start = high_resolution_clock::now();

		score = negamax(position, counter);

		// now take note of the time again
		high_resolution_clock::time_point stop = high_resolution_clock::now();

		// calculate the time taken for execution
		microseconds duration = chrono::duration_cast<microseconds>(stop - start);

		cout << line << " " << score << " " << counter << " " << duration.count() << endl;
	}

	return 0;
}

int only_digits(const string& line) {
	return all_of(line.begin(), line.end(), ::isdigit);
}

int negamax(const Position& P, int& position_counter) {
	// Increment the position counter as we are evaluating a new position
	position_counter++;

	// Check for a draw, and return 0 if the case
	if (P.get_moves() == Position::WIDTH * Position::HEIGHT) return 0;

	// Check if current player can win in the next move, and return appropriate
	// score if the case
	for (int i = 0; i < Position::WIDTH; i++)
		if (P.can_play(i) && P.is_winning_move(i))
			return (Position::WIDTH * Position::HEIGHT - P.get_moves() + 1) / 2;

	// Otherwise, recursively evaluate future positions via negamax and use
	// those evaluations to compute the value of the current position
	
	// In order to keep track of which of the positions in the immediate future
	// is the most favourable to the current player, create a variable to store
	// the best score produced by the immediate future positions. Initialise
	// this variable using the lower bound of the possible score.
	int bestScore = -(Position::WIDTH * Position::HEIGHT) / 2;

	// Evaluate the scores of all possible next positions and keep the best one
	for (int i = 0; i < Position::WIDTH; i++)
		if (P.can_play(i)) {
			// Create a copy of the curent position
			Position P2(P);
			// Use this copy to play the potential move
			P2.play(i);
			// Evaluate the position (negative score because the "current player"
			// in this position would be the opponent of the current player of
			// the current position being evaluated.
			int score = -negamax(P2, position_counter);
			// Store this score in the bestScore variable if this is the best
			// of the scores seen so far.
			if (score > bestScore) bestScore = score;
		}

	// Return the best score reached through this manner
	return bestScore;
}
