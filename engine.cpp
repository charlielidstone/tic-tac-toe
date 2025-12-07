#include <limits>

#include "engine.hpp"

Engine::Engine() {
}

int Engine::minimax(Board board, bool isMax, int depth) {
    int score = board.evaluate();

    // we subract the depth because we want to prioritise the moves that are closest to the top of the tree
    if (score == 10) return score - depth;
    if (score == -10) return score + depth;

    if (!board.isMovesLeft()) return 0;

    if (isMax) {
        int bestScore = std::numeric_limits<int>::max();

        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (board.getCell(row, col) == ' ') {
                    board.setCell(row, col, 'X');
                    int val = minimax(board, false, depth + 1);
                    board.setCell(row, col, ' ');
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