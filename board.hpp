#ifndef BOARD_HPP
#define BOARD_HPP

#include <array>
#include <string>

class Board {
    public:
        Board();
	    Board(const std::array<std::array<char, 3>, 3> &initialBoard);
        void print();
	    void printExampleBoard();
        char getCell(int row, int col) const;
        char getCell(int squareNum) const;
	    int isMovesLeft() const;
        /**
        * @brief evaluate() determines the state of the game for the given board
        * @param board, reference to 2D array representing 3x3 game board
        * @return 10 if the board is a win for O, -10 if it is a win for X, 0 otherwise
        */
        int evaluate() const;
        enum UpdateStatus {
            success,
            failSpaceOccupied,
            failInvalidInput,
            failUnknownError,
        };
		UpdateStatus updateBoard(int squareNum, char value);
        void handleError(Board::UpdateStatus updateStatus, int squareNum, std::string* errorMessage);
        void setCell(int row, int col, char value);
	    void setCell(int squareNum, char value);
    private:
        std::array<std::array<char, 3>, 3> grid{ {} };
};

#endif