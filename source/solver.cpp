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
 * @version: 1.2.0
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
 * This variable will contain column values in the order that they should be
 * explored (from the centre columns). The values will be initialised properly
 * within the main function.
 */
int columnOrder[Position::WIDTH];

/**
 * Checks if a given line (string) contains only digit chars.
 * @return 1 if string contains only digits, 0 otherwise
 */
int only_digits(const string& line);

/**
 * Recursively solve a connect4 position using the negamax variant of the
 * minimax algorithm with alpha-beta pruning. A reference to an int counter
 * should be passed in, which will be incremented every time a new position is 
 * evaluated in the recursive algorithm.
 * @param alpha, the lower bound for the window in which we search for the
 *        score, which also represents the best score reached upstream so far
 *        by the maximiser.
 * @param beta, the upper bound for the window in which we search for the score,
 *        which also represents the best score reached upstream so far by the
 *        minimiser.
 * @return the exact score, or an upper or lower bound of the socre of a
 *         position depending on the case:
 *  - if alpha <= actual score <= beta, then return true score
 *  - if actual score <= alpha, then return upper bound of actual score
 *  - if actual score >= beta, then return lower bound of actual score
 *  - the scores will take the following values:
 *		- 0 for a draw
 *		- positive score for a forcing win, where the numerical value
 *		  corresponds to the number of turns before the maximum possible turns
 *		  that you win (that is, the earlier you win, the higher your score)
 *		- negative score for a forcing loss, where the numerical value
 *		  corresponds to the number of turns before the maximum possible turns
 *		  that you lose (that is, the earlier you lose, the lower your score)
 */
int negamax(const Position& P, int alpha, int beta, int& position_counter);

/**
 * read every line from the standard input, which should contain an encoding
 * of a connect4 position (read above for the position encoding syntax), evaluate
 * that position, and return that position's score, number of evaluated positions,
 * and computation time in microseconds.
 */
int main() {
	// Initialise the values within the columnOrder array.
	for (int i = 0; i < Position::WIDTH; i++)
		columnOrder[i] = Position::WIDTH/2 + (i+1)/2 * (1-2*(i%2));

	// Declare a string to store the read lines in, a position object to store
	// positions in, and ints for the score and a counter to track the number
	// of positions explored
	string line;
	Position position;
	int score, counter, baseScore = Position::WIDTH * Position::HEIGHT / 2;

	while (getline(cin, line)) {
		if (!only_digits(line))
			throw runtime_error("Input contains lines with non-digit charcters.");
		counter = 0;
		position = Position(line);

		// take a note of the time to measure execution time in microseconds
		high_resolution_clock::time_point start = high_resolution_clock::now();

		score = negamax(position, -baseScore, baseScore, counter);

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

int negamax(const Position& P, int alpha, int beta, int& position_counter) {
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
	
	// Upper bound the max possible score, given that we cannot win immediately
	int maxScore = (Position::WIDTH * Position::HEIGHT - P.get_moves() - 1) / 2;
	
	// Beta does not need to be larger than maxScore
	if (beta > maxScore) {
		beta = maxScore;
		// Prune exploration if the [alpha:beta] window is now empty
		if (alpha >= beta) return beta;
	}

	// Evaluate the scores of all possible next positions and keep the best one
	for (int i = 0; i < Position::WIDTH; i++) {
		int move = columnOrder[i];
		if (P.can_play(move)) {
			// Create a copy of the curent position
			Position P2(P);
			// Use this copy to play the potential move
			P2.play(move);
			// Evaluate the position (negative score because the "current player"
			// in this position would be the opponent of the current player of
			// the current position being evaluated. Notice also that the alpha
			// and beta are inverted and fed in in the opposite order.)
			int score = -negamax(P2, -beta, -alpha, position_counter);
			// Prune the exploration if we find a move better than what our
			// opponent will allow (beta)
			if (score >= beta) return score;
			// Reduce the [alpha:beta] window for subsequent exploration if we
			// find current_alpha < score < beta.
			if (score > alpha) alpha = score;
		}
	}

	// Return the minimum guaranteed score
	return alpha;
}
