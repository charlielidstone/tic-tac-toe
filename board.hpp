#include <iostream>
#include <array>


class Board {
private:
    std::array<std::array<char, 3>, 3> board;

public:
    Board() {
        for (auto& row : board) {
            row.fill(' ');
        }
    }

    Board(const std::array<std::array<char, 3>, 3>& initialBoard) {
        board = initialBoard;
    }

    void print() const {
        for (int row = 0; row < 3; row++) {
            std::cout << " " << board[row][0] << " | "
                << board[row][1]
                << " | " << board[row][2] << " \n";
            if (row < 2) {
                std::cout << "---+---+---\n";
            }
        }
    }
};