#include <iostream>
#include <array>
#include <string>
#include "board.hpp"
#include "utils.hpp"

Board::Board() : updateStatus(notUpdated) {
	for (auto &row : grid) {
		row.fill(' ');
	}
}

Board::Board(const std::array<std::array<char, 3>, 3> &initialBoard) : updateStatus(notUpdated), grid(initialBoard) {}
/*
void Board::print() {
    const std::string grey = "\033[38;2;80;80;80m";
    const std::string reset = "\033[0m";

    //std::cout << "\033[10B";

    for (int row = 0; row < grid.size(); row++) {
        std::cout << "\033[50C";
		for (int col = 0; col < grid.size(); col++) {
            if (grid[row][col] == ' ') {
                std::cout << " " << grey << std::to_string(row * 3 + col + 1) << reset << " |";
            } else {
                std::cout << " " << grid[row][col] << " |";
            }
            if (col == grid.size() - 1) {
                std::cout << "\b";
			}
        }
        std::cout << " \n";
        std::cout << "\033[50C";
        if (row < grid.size() - 1) {
            std::cout << "---+---+---\n";
        }
    }
}

void Board::printExampleBoard() {
    const std::string grey = "\033[38;2;80;80;80m";
    const std::string reset = "\033[0m";

    std::cout
        << reset << " " << grey << "1" << reset << " | "
        << grey << "2" << reset << " | "
        << grey << "3" << reset << "\n"

        << reset << "---+---+---" << reset << "\n"

        << reset << " " << grey << "4" << reset << " | "
        << grey << "5" << reset << " | "
        << grey << "6" << reset << "\n"

        << reset << "---+---+---" << reset << "\n"

        << reset << " " << grey << "7" << reset << " | "
        << grey << "8" << reset << " | "
        << grey << "9" << reset << "\n\n";
}
*/
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
    if (squareNum < 1 || squareNum > 9 || isalpha(squareNum)) {
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
        *errorMessage = std::to_string(squareNum) + " is already taken!";
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