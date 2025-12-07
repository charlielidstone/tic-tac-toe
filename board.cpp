#include <iostream>
#include <array>
#include <string>
#include "board.hpp"
#include "utils.hpp"

Board::Board() {
	for (auto &row : grid) {
		row.fill(' ');
	}
}

Board::Board(const std::array<std::array<char, 3>, 3> &initialBoard) {
    grid = initialBoard;
}

void Board::print() {
    for (int row = 0; row < grid.size(); row++) {
		for (int col = 0; col < grid.size(); col++) {
            std::cout << " " << grid[row][col] << " |";
            if (col == grid.size() - 1) {
                std::cout << "\b";
			}
        }
        std::cout << " \n";
        if (row < grid.size() - 1) {
            std::cout << "---+---+---\n";
        }
    }
}

void Board::printExampleBoard() {
    std::cout << " 1 | 2 | 3\n---+---+---\n 4 | 5 | 6\n---+---+---\n 7 | 8 | 9 \n\n";
}

int Board::isMovesLeft() const {
	for (auto& row : grid) {
        for (auto &cell : row) {
            if (cell == ' ') {
                return true;
            }
        }
    }
    return false;
}

char Board::getCell(int row, int col) const {
    return grid[row][col];
}

char Board::getCell(int index) const {
	int row = index / grid.size() + 1; // not sure if the + 1 is correct here
    int col = index % grid.size() + 1;
    return grid[row][col];
}

void Board::setCell(int row, int col, char value) {
    grid[row][col] = value;
}

void Board::setCell(int squareNum, char value) {
	std::pair<int, int> position = utils::getPair(squareNum);
    grid[position.first][position.second] = value;
}

Board::UpdateStatus Board::updateBoard(int squareNum, char value) {
    std::pair<int, int> position = utils::getPair(squareNum);
    int row = position.first;
    int col = position.second;
    if (squareNum < 1 || squareNum > 9) {
        return failInvalidInput;
    }
    if (grid[row][col] != ' ') {
        return failSpaceOccupied;
    }
    grid[row][col] = value;
    return success;
}

void Board::handleError(Board::UpdateStatus updateStatus, int squareNum, std::string* errorMessage) {
    if (updateStatus == Board::UpdateStatus::failSpaceOccupied) {
        *errorMessage = "\n" + std::to_string(squareNum) + " is already taken!";
    }
    else if (updateStatus == Board::UpdateStatus::failInvalidInput) {
        *errorMessage = "\nPlease enter a single digit from 1 to 9.";
    }
    else {
        *errorMessage = "\nSome unknown error occurred. :(";
    }
}

int Board::evaluate() const {
    for (int row = 0; row < 3; row++) {
        if (getCell(row, 0) != ' ' &&
            getCell(row, 0) == getCell(row, 1) &&
            getCell(row, 1) == getCell(row, 2)) {
            return getCell(row, 0) == 'O' ? +10 : -10;
        }
    }
    for (int col = 0; col < 3; col++) {
        if (getCell(0, col) != ' ' &&
            getCell(0, col) == getCell(1, col) &&
            getCell(1, col) == getCell(2, col)) {
            return getCell(0, col) == 'O' ? +10 : -10;
        }
    }
    if (getCell(0, 0) != ' ' &&
        getCell(0, 0) == getCell(1, 1) &&
        getCell(1, 1) == getCell(2, 2)) {
        return getCell(0, 0) == 'O' ? +10 : -10;
    }
    if (getCell(0, 2) != ' ' &&
        getCell(0, 2) == getCell(1, 1) &&
        getCell(1, 1) == getCell(2, 0)) {
        return getCell(0, 2) == 'O' ? +10 : -10;
    }

    return 0;
}