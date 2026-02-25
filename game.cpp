#include <iostream>
#include <chrono>
#include <thread>
#include "game.hpp"
#include "board.hpp"
#include "utils.hpp"
#include "engine.hpp"
#include "player.hpp"

Game::Game(Renderer& renderer) : isBotGame(false), botStarts(false), renderer(renderer) {
    player1 = std::make_unique<HumanPlayer>('X');
    player2 = std::make_unique<HumanPlayer>('O');
    currentPlayer = player1.get();
}

Game::Game(bool botStarts, Renderer& renderer) : isBotGame(true), botStarts(botStarts), renderer(renderer) {
    if (botStarts) {
        player1 = std::make_unique<ComputerPlayer>('O');
        player2 = std::make_unique<HumanPlayer>('X');
    }
    else {
        player1 = std::make_unique<HumanPlayer>('X');
        player2 = std::make_unique<ComputerPlayer>('O');
    }
    currentPlayer = player1.get();
}

void Game::displayStartingScreen() {
    /*
    int gameType = 0;
    
    while (gameType != '1' && gameType != '2' && gameType != '3') {
        renderer.clearScreen();
        renderer.renderStartingScreen();
        renderer.renderText("Please enter 1, 2, or 3 to select a game mode.");
        gameType = std::cin.get();
    }

    if (gameType == '1') {
		isBotGame = false;
        player1 = std::make_unique<HumanPlayer>('X');
        player2 = std::make_unique<HumanPlayer>('O');
    }
    else if (gameType == '2') {
		isBotGame = true;
		botStarts = true;
        player1 = std::make_unique<ComputerPlayer>('O');
        player2 = std::make_unique<HumanPlayer>('X');
    }
    else if (gameType == '3') {
		isBotGame = true;
        botStarts = false;
        player1 = std::make_unique<HumanPlayer>('X');
        player2 = std::make_unique<ComputerPlayer>('O');
    }*/

    start(renderer);
}

void Game::start(Renderer &renderer) {

    isOver = false;

    Board board;

    std::string errorMessage{ "" };

    do {
		board.updateStatus = Board::UpdateStatus::notUpdated;

        int squareNum{};

		std::string promptMessage = currentPlayer->getName() + " (" + currentPlayer->getSymbol() + "), enter your move (1-9): ";

		renderer.clearScreen();
		renderer.renderPlayingScreen(board, errorMessage, promptMessage);

		squareNum = currentPlayer->prompt(board, renderer, promptMessage);
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

    renderer.clearScreen();
    //board.printExampleBoard();
    //board.print();
    if (board.evaluate() == +10) {
        //std::cout << "\nGood game! Player 2 wins!" << std::endl;
		//renderer.renderText("Good game! Player 2 wins!");
		renderer.renderGameOverScreen(board, 'O');
    }
    else if (board.evaluate() == -10) {
        //std::cout << "\nGood game! Player 1 wins!" << std::endl;
		//renderer.renderText("Good game! Player 1 wins!");
        renderer.renderGameOverScreen(board, 'X');
    }
    else if (board.evaluate() == 0) {
        //std::cout << "\nGood game! It's a TIE!" << std::endl;
		//renderer.renderText("Good game! It's a TIE!");
        renderer.renderGameOverScreen(board, 'T');
    }
    else {
        //std::cout << "\nGood game!" << std::endl;
		//renderer.renderText("Good game!");
		renderer.renderGameOverScreen(board, 'T');
    }

    // Keep running until 'q' is pressed
    //std::cout << "Press 'q' and Enter to exit..." << std::endl;
	renderer.renderText("Press 'q' and Enter to exit...");
    char input;
    while (std::cin >> input) {
        if (input == 'q' || input == 'Q') {
            break;
        }
        std::cout << "Press 'q' and Enter to exit..." << std::endl;
    }
}