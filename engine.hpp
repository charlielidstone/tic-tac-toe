#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <utility>
#include "board.hpp"

class Engine {
public:
	Engine();
	std::pair<int, int> findBestMove(Board board);
	
private:
	/**
	* @brief returns a score for a single board position by repeatedly calling evaluateBoard() until a win, draw, or loss is reached
	* @param board, reference to 2D array representing the game board
	* @param isMax, boolean for whether or not we should maximise the score. We want to maximise for our score, minimise for the opponents
	* @param depth, an integer representing the recursive level of the function call
	* @return an integer representing the score the the given position, 0 means the position results in a draw
	*/
	int minimax(Board board, bool isMax, int depth);
	int artificialDelay = 100;

};

#endif