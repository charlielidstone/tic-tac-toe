#include "board.hpp"
#include <iostream>
#include <array>

Board::Board() {
	for (auto &row : grid) {
		row.fill(' ');
	}
}

Board::Board(const std::array<std::array<char, 3>, 3> &initialBoard) {
    grid = initialBoard;
}

void Board::print() {
    for (int row = 0; row < 3; row++) {
        std::cout << " " << grid[row][0] << " | "
            << grid[row][1]
            << " | " << grid[row][2] << " \n";
        if (row < 2) {
            std::cout << "---+---+---\n";
        }
    }
}

int Board::isMovesLeft() const {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (grid[i][j] == ' ') {
                return true;
            }
        }
    }
    return false;
}