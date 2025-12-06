#include <limits>

#include "engine.hpp"

Engine::Engine() {
}

int Engine::evaluateBoard(Board board) {
    for (int row = 0; row < 3; row++) {
        if (board[row][0] != ' ' &&
            board[row][0] == board[row][1] &&
            board[row][1] == board[row][2]) {
            return board[row][0] == 'O' ? +10 : -10;
        }
    }
    for (int col = 0; col < 3; col++) {
        if (board[0][col] != ' ' &&
            board[0][col] == board[1][col] &&
            board[1][col] == board[2][col]) {
            return board[0][col] == 'O' ? +10 : -10;
        }
    }
    if (board[0][0] != ' ' &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2]) {
        return board[0][0] == 'O' ? +10 : -10;
    }
    if (board[0][2] != ' ' &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0]) {
        return board[0][2] == 'O' ? +10 : -10;
    }

    return 0;
}

int Engine::minimax(Board board, bool isMax, int depth) {
    int score = evaluateBoard(board);

    // we subract the depth because we want to prioritise the moves that are closest to the top of the tree
    if (score == 10) return score - depth;
    if (score == -10) return score + depth;

    if (!Board::isMovesLeft()) return 0;

    if (isMax) {
        int bestScore = std::numeric_limits<int>::max();

        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (board[row][col] == ' ') {
                    board[row][col] = 'X';
                    int val = minimax(board, false, depth + 1);
                    board[row][col] = ' ';
                    if (val <= bestScore) {
                        bestScore = val;
                    }
                }
            }
        }
        return bestScore;
    }
    else {
        int bestScore = std::numeric_limits<int>::min();

        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (board.getCell(row, col) == ' ') {
                    board.setCell(row, col, 'O');
                    int val = minimax(board, true, depth + 1);
					board.setCell(row, col, ' ');
                    if (val >= bestScore) {
                        bestScore = val;
                    }
                }
            }
        }
        return bestScore;
    }
}

std::pair<int, int> Engine::findBestMove(Board board) {
    std::pair<int, int> bestMove = { -1, -1 };
    int bestScore = std::numeric_limits<int>::min();
    int score{};
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (board.getCell(row, col) == ' ') {
                board.setCell(row, col, 'O');
                score = minimax(board, true, 1);
                board.setCell(row, col, ' ');
                if (score >= bestScore) {
                    bestScore = score;
                    bestMove = { row, col };
                }
            }
        }
    }
    return bestMove;
}