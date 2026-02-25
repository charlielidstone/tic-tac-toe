#include <limits>
#include <thread>
#include <chrono>
#include "utils.hpp"
#include "engine.hpp"

Engine::Engine() {
}

int Engine::minimax(Board board, bool isMax, int depth) {
    int score = board.evaluate();

    // we subract the depth because we want to prioritise the moves that are closest to the top of the tree
    if (score == 10) return score - depth;
    if (score == -10) return score + depth;

    if (!board.isMovesLeft()) return 0;

    int bestScore = isMax ? std::numeric_limits<int>::max() : std::numeric_limits<int>::min();

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (board.getCell(row, col) == ' ') {
                
                board.setCell(row, col, isMax ? 'X' : 'O');
                int val = minimax(board, !isMax, depth + 1);
                board.setCell(row, col, ' ');
                
                if (val <= bestScore && isMax) {
                    bestScore = val;
                }
                else if (val >= bestScore && !isMax) {
                    bestScore = val;
                }
            }
        }
    }
    return bestScore;
}

std::pair<int, int> Engine::findBestMove(Board board) {
	std::chrono::milliseconds time(artificialDelay);
	std::this_thread::sleep_for(time);
    std::pair<int, int> bestMove = { -1, -1 };
    int bestScore = std::numeric_limits<int>::min();
    int score{};
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (board.getCell(row, col) == ' ') {
                board.setCell(row, col, 'O');
                score = minimax(board, true, 0);
                utils::log("log.txt", board.toString(false), true);
                utils::log("log.txt", "score: " + std::to_string(score) + "\n", true);
                board.setCell(row, col, ' ');
                if (score >= bestScore) {
                    bestScore = score;
                    bestMove = { row, col };
                }
            }
        }
    }
	utils::log("log.txt", "best score: " + std::to_string(bestScore) + "\n", true);
	utils::log("log.txt", "best move: " + std::to_string(bestMove.first) + ", " + std::to_string(bestMove.second) + "\n", true);
	utils::log("log.txt", "-----------------------------\n", true);
    return bestMove;
}