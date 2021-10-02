/**
	position.hpp
	Purpose: A definition for a class storing a Connect 4 position.

	@author Yuta Nagano
	@version 0.0.1
*/

/**
A class storing a Connect 4 position.
Functions are relative to the current player.
*/
class Position {
	private:
		int board[WIDTH][HEIGHT];
		int heights[WIDTH];
		unsigned int moves;
	public:
		static const int WIDTH = 7;
		static const int HEIGHT = 6;

		/**
		Default constructor, build an empty position.
		*/
		Position();

		/**
		Constructor taking in a char* of moves to construct a mid-game position.
		If the input string does not produce a valid position, this will throw an error.
		*/
		Position(char* moves);

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
		@return number of moves played in the current game
		*/
		unsigned int num_moves() const;
};
