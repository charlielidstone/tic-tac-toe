#ifndef BOARD_HPP
#define BOARD_HPP

#include <array>

class Board {
private:
    std::array<std::array<char, 3>, 3> grid;

public:
    Board();
	Board(const std::array<std::array<char, 3>, 3> &initialBoard);
    void print();
    char getCell(int row, int col) const {
        return grid[row][col];
	}
    char getCell(int index) const {
        int row = index / 3 + 1;
        int col = index % 3 + 1;
		return grid[row][col];
	}
    void setCell(int row, int col, char value) {
        grid[row][col] = value;
	}
	int isMovesLeft() const;
};

#endif