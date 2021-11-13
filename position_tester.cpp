/**
 * position_tester.cpp
 * Purpose: Unit test for the position class.
 *
 * @author Yuta Nagano
 * @version 1.0.0
 */

#include <iostream>
#include <string>
#include "position.hpp"

using namespace std;

int fail(string msg);

int main() {
	// Create an instance of a Position
	Position test;

	// Test if the Position instance has been initialised correctly
	for (int col = 0; col<test.WIDTH; col++) {
		if (test.get_height(col) != 0) {
			return fail("Default constructor didn't initialise heights array correctly.");
		}

		for (int row = 0; row<test.HEIGHT; row++) {
			if (test.get_board(col, row) != 0) {
				return fail("Default constructor didn't initialise board correctly.");
			}
		}
	}

	if (test.get_moves() != 0) {
		return fail("Default constructor didn't initialise move counter correctly.");
	}

	// Check that passing a bad col to can_play() throws an error
	// First, passing good cols should not throw any errors, and should return
	// true for all cols tested as Position instance should be empty.
	try {
		for (int col = 0; col<test.WIDTH; col++) {
			if (!test.can_play(col)) {
				return fail("can_play() returned false for an empty column.");
			}
		}
	}
	catch (...) {
		return fail("can_play() threw an error with valid cols.");
	}

	// Make sure that the method returns errors when passing bad cols
	try {
		test.can_play(-1);
		return fail("can_play(-1) did not throw an error.");
	}
	catch (...) {}

	try {
		test.can_play(test.WIDTH);
		return fail("can_play(test.WIDTH) did not throw an error.");
	}
	catch (...) {}

	// Test the play() method
	test.play(3);

	// First, check after one play call
	if (test.get_height(3) != 1) {
		return fail("play(3) did not increase col 3 height. (call 1)");
	}
	if (test.get_board(3, 0) != -1) { // -1 because flip_board() would have been called
		return fail("play(3) did not reflect on the board array. (call 1)");
	}
	if (test.get_moves() != 1) {
		return fail("play(3) did not reflect on move counter. (call 1)");
	}

	// Then check after two calls
	test.play(3);

	if (test.get_height(3) != 2) {
		return fail("play(3) did not increase col 3 height. (call 2)");
	}
	if (test.get_board(3, 1) != -1 || test.get_board(3, 0) != 1) {
		return fail("play(3) did not reflect on the board array. (call 2)");
	}
	if (test.get_moves() != 2) {
		return fail("play(3) did not reflect on move counter. (call 2)");
	}

	// Then check after three calls
	test.play(2);

	if (test.get_height(2) != 1) {
		return fail("play(2) did not increase col 2 height. (call 3)");
	}
	if (test.get_board(3, 1) != 1 || test.get_board(3, 0) != -1 || test.get_board(2, 0) != -1) {
		return fail("play(2) did not reflect on the board array. (call 3)");
	}
	if (test.get_moves() != 3) {
		return fail("play(2) did not reflect on move counter. (call 3)");
	}

	// Ensure can_play() method returns false on full column
	// Fill col 4 with pieces until full
	for (int i = 0; i<test.HEIGHT; i++) test.play(4);

	// Ensure can_play(4) now returns false
	if (test.can_play(4)) return fail("can_play() returns true on full column.");

	// Test the alternative constructor
	test = Position("4453");
	/*
	 * | | | | | | | |
	 * | | | | | | | |
	 * | | | | | | | |
	 * | | | | | | | |
	 * | | | |-| | | |
	 * | | |-|+|+| | |
	 */
	if (test.get_board(2, 0) != -1 || \
			test.get_board(3, 0) != 1 || \
			test.get_board(3, 1) != -1 || \
			test.get_board(4, 0) != 1) {
		return fail("Alternate constructor failed to initialise board. (4453)");
	}
	if (test.get_height(2) != 1 || \
			test.get_height(3) != 2 || \
			test.get_height(4) != 1) {
		return fail("Alternate constructor failed to initialise heights. (4453)");
	}
	if (test.get_moves() != 4) {
		return fail("Alternate constructor failed to initialise move counter. (4453)");
	}

	test = Position("44455554221");
	/*
	 * | | | | | | | |
	 * | | | | | | | |
	 * | | | |+|-| | |
	 * | | | |-|+| | |
	 * | |+| |+|-| | |
	 * |-|-| |-|+| | |
	 */
	if (test.get_board(0, 0) != -1 || \
			test.get_board(1, 0) != -1 || \
			test.get_board(1, 1) != 1 || \
			test.get_board(3, 0) != -1 || \
			test.get_board(3, 1) != 1 || \
			test.get_board(3, 2) != -1 || \
			test.get_board(3, 3) != 1 || \
			test.get_board(4, 0) != 1 || \
			test.get_board(4, 1) != -1 || \
			test.get_board(4, 2) != 1 || \
			test.get_board(4, 3) != -1) {
		return fail("Alternative constructor failed to initialise board. (44455554221i)");
	}
	if (test.get_height(0) != 1 || \
			test.get_height(1) != 2 || \
			test.get_height(3) != 4 || \
			test.get_height(4) != 4) {
		return fail("Alternative constructor failed to initialise heights. (44455554221)");
	}
	if (test.get_moves() != 11) {
		return fail("Alternative constructor failed to initialise move counter. (44455554221)");
	}

	// Test the is_winning_move() method
	test.play(5);

	// Current position stored by test:
	/*
	 * | | | | | | | |
	 * | | | | | | | |
	 * | | | |-|+| | |
	 * | | | |+|-| | |
	 * | |-| |-|+| | |
	 * |+|+| |+|-|-| |
	 */
	if (!test.is_winning_move(2)) return fail("is_winning_move() did not detect a horizontal win-in-1.");

	test.play(0);
	test.play(2);
	// Current position stored by test:
	/*
	 * | | | | | | | |
	 * | | | | | | | |
	 * | | | |-|+| | |
	 * | | | |+|-| | |
	 * |+|-| |-|+| | |
	 * |+|+|-|+|-|-| |
	 */
	if (!test.is_winning_move(2)) return fail("is_winning_move() did not detect a diagonal win-in-1.");

	test.play(0);
	test.play(2);
	// Current position stored by test:
	/*
	 * | | | | | | | |
	 * | | | | | | | |
	 * | | | |-|+| | |
	 * |+| | |+|-| | |
	 * |+|-|-|-|+| | |
	 * |+|+|-|+|-|-| |
	 */
	if (!test.is_winning_move(0)) return fail("is_winning_move() did not detect a vertical win-in-1.");

	test.play(4);
	test.play(0);
	/*
	 * | | | | | | | |
	 * | | | | |+| | |
	 * |-| | |-|+| | |
	 * |+| | |+|-| | |
	 * |+|-|-|-|+| | |
	 * |+|+|-|+|-|-| |
	 */
	for (int i = 0; i<test.WIDTH; i++) {
		if (test.is_winning_move(i)) return fail("is_winning_move() falsely detected a win-in-1 when there were none.");
	}

	// If no errors have been found, test passed!
	cout << "Test passed!" << endl;
	return 0;
}

int fail(string msg) {
	cout << "Test failed: " << msg << endl;
	return 1;
}
