#include <iostream>
#include <chrono>
#include <thread>
#include "game.hpp"
#include "board.hpp"
#include "utils.hpp"
#include "engine.hpp"
#include "player.hpp"

Game::Game() : isBotGame(false), botStarts(false) {
    player1 = std::make_unique<HumanPlayer>('X');
    player2 = std::make_unique<HumanPlayer>('O');
    currentPlayer = player1.get();
}

Game::Game(bool botStarts) : isBotGame(true), botStarts(botStarts) {
    if (botStarts) {
        player1 = std::make_unique<ComputerPlayer>('X');
        player2 = std::make_unique<HumanPlayer>('O');
    }
    else {
        player1 = std::make_unique<HumanPlayer>('X');
        player2 = std::make_unique<ComputerPlayer>('O');
    }
    currentPlayer = player1.get();
}

void Game::start() {
    isOver = false;

    Board board;

    std::string errorMessage{ "" };

    do {
		board.updateStatus = Board::UpdateStatus::notUpdated;

        int squareNum{};

        utils::clearScreen();
        board.printExampleBoard();
        board.print();
        std::cout << errorMessage << std::endl;

		squareNum = currentPlayer->prompt(board);
        Board::UpdateStatus updateStatus = board.updateBoard(squareNum, currentPlayer->getSymbol());
        if (updateStatus == Board::UpdateStatus::success) {
            errorMessage = "";
			currentPlayer = (currentPlayer == player1.get()) ? player2.get() : player1.get();
        }
        else {
			board.handleError(updateStatus, squareNum, &errorMessage);
        }

        int eval = board.evaluate();
        isOver = (eval != 0)  || !board.isMovesLeft();

    } while (!isOver);

    utils::clearScreen();
    board.printExampleBoard();
    board.print();
    if (board.evaluate() == +10) {
        std::cout << "\nGood game! Player 2 wins!" << std::endl;
    }
    else if (board.evaluate() == -10) {
        std::cout << "\nGood game! Player 1 wins!" << std::endl;
    }
    else if (board.evaluate() == 0) {
        std::cout << "\nGood game! It's a TIE!" << std::endl;
    }
    else {
        std::cout << "\nGood game!" << std::endl;
    }
}