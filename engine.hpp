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
	* @return an integer representing the score the the given position
	*/
	int minimax(Board board, bool isMax, int depth);
    
	/**
	* @brief determines the state of the game for the given board
	* @param board, reference to 2D array representing 3x3 game board
	* @return 10 if the board is a win for O, -10 if it is a win for X, 0 otherwise
	*/
	int evaluateBoard(Board board);
};