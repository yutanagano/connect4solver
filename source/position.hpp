/**
	position.hpp
	Purpose: A definition for a class storing a Connect 4 position.

	@author Yuta Nagano
	@version 1.0.0
*/

#ifndef POSITION_HEADER
#define POSITION_HEADER

#include<string>

using namespace std;

/**
A class storing a Connect 4 position.
Functions are relative to the current player.
*/
class Position {

	public:
		static const int WIDTH = 7;
		static const int HEIGHT = 6;

		/**
		Default constructor, build an empty position.
		*/
		Position();

		/**
		Constructor taking in a string of moves to construct a mid-game position.
		Note: the string of moves is 1-indexed, not 0-indexed. This is in contrast
		to the rest of the methods.
		If the input string does not produce a valid position, this will throw an error.
		*/
		Position(string moves);

		/**
		Indicates whether a column is playable.
		@param col: 0-based index of column to play
		@return true if column playable, false otherwise
		*/
		bool can_play(int col) const;

		/**
		Plays a playable column.
		This function should not be called on an unplayable column.
		@param col: 0-based index of column to play
		*/
		void play(int col);

		/**
		Indicates whether current player wins playing a given column.
		This function should not be called on an unplayable column.
		@param col: 0-based index of column to play
		@return true if winning move, false otherwise
		*/
		bool is_winning_move(int col) const;

		/**
		 * @return a value from a specified index on the board array
		 */
		int get_board(int col, int row) const;

		/**
		 * @return a value from a specified index on the heights array
		 */
		int get_height(int col) const;

		/**
		@return number of moves played in the current game
		*/
		unsigned int get_moves() const;

	private:
		int board[WIDTH][HEIGHT];
		int heights[WIDTH];
		unsigned int moves;

		/**
		 * Change the perspective of the board so that the current player
		 * switches.
		 */
		void flip_board();

};

#endif
