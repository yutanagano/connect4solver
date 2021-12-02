/**
 * position.cpp
 * Purpose: Implementation for a class storing a Connect4 position.
 *
 * @author Yuta Nagano
 * @version 1.0.1
 */

#include <stdexcept>
#include <cassert>
#include <string>
#include <algorithm>
#include <cctype>
#include "position.hpp"

using namespace std;

// Constructors

Position::Position() : board{0}, heights{0}, moves{0} {}

Position::Position(string moves) : board{0}, heights{0}, moves{0} {
	// Assert that the string moves only contains digits
	if (!all_of(moves.begin(), moves.end(), ::isdigit))
		throw runtime_error("a string containing non-digit characters cannot be passed to Position constructor.");
	int move;
	for (int i = 0; i<moves.length(); i++) {
		move = moves[i] - '0' - 1;
		play(move);
	}
}

// Public methods

bool Position::can_play(int col) const {
	// If user enters non-existent col, throw error
	if (col >= WIDTH) throw runtime_error("can_play cannot evaluate at col >= WIDTH.");
	if (col < 0) throw runtime_error("can_play cannot evaluate at col < 0.");

	// Return true if the height of the specified column is less than HEIGHT
	return heights[col] < HEIGHT;
}

void Position::play(int col) {
	// Make sure that the specified column is playable
	assert(can_play(col));

	// Add a current player piece (1) to the specified column at the appropriate height
	board[col][heights[col]] = 1;

	// Increment the appropriate entry in the heights array by 1
	heights[col]++;

	// Increment the move number counter by 1
	moves ++;

	// Flip the board to 'change' the current player
	flip_board();
}

bool Position::is_winning_move(int col) const {
	// Check that the col entered is legal
	assert(can_play(col));

	// First, note the coordinates of the next play (we already have col, get row)
	int row = heights[col];

	// Now check for wins horizontally
	int count0 = 0, count1 = 0;

	for (int i=col-1; i>=0; i--) {
		if (board[i][row] == 1) count0++;
		else break;
	}
	for (int i=col+1; i<WIDTH; i++) {
		if (board[i][row] ==1) count1++;
		else break;
	}

	if (count0 + count1 >= 3) return true;

	count0 = 0, count1 = 0;

	// Now check for wins vertically
	for (int i=row-1; i>=0; i--) {
		if (board[col][i] == 1) count0++;
		else break;
	}
	for (int i=row+1; i<HEIGHT; i++) {
		if (board[col][i] == 1) count1++;
		else break;
	}

	if (count0 + count1 >= 3) return true;

	count0 = 0, count1 = 0;

	// Now check for diagnoal wins
	for (int i=-1; col+i >=0 && row+i >= 0; i--) {
		if (board[col+i][row+i] == 1) count0++;
		else break;
	}
	for (int i=1; col+i < WIDTH && row+i < HEIGHT; i++) {
		if (board[col+i][row+i] == 1) count1++;
		else break;
	}

	if (count0 + count1 >= 3) return true;
	
	count0 = 0, count1 =0;

	for (int i=-1; col+i >=0 && row-i < HEIGHT; i--) {
		if (board[col+i][row-i] == 1) count0++;
		else break;
	}
	for (int i=1; col+i < WIDTH && row-i >= 0; i++) {
		if (board[col+i][row-i] == 1) count1++;
		else break;
	}

	if (count0 + count1 >= 3) return true;

	// No horizontal, vertical, nor diagonal wins found, so return false
	return false;
}

int Position::get_board(int col, int row) const {
	return board[col][row];
}

int Position::get_height(int col) const {
	return heights[col];
}

unsigned int Position::get_moves() const {
	return moves;
}

// Private methods

void Position::flip_board() {
	// Replace all 1's (current player pieces) with -1's (opponent player pieces)
	// and vice versa
	for (int i=0; i<WIDTH; i++) {
		for (int j=0; j<HEIGHT; j++) {
			board[i][j] = -board[i][j];
		}
	}
}
