#include <iostream>
#include "player.hpp"
#include "engine.hpp"
#include "utils.hpp"

Player::Player() : type(HUMAN_X), name("Player 1"), symbol('X') {}
Player::Player(PlayerType playertype) : type(playertype) {
	switch (playertype) {
		case HUMAN_O:
			name = "Player 2";
			symbol = 'O';
			break;
		case HUMAN_X:
			name = "Player 1";
			symbol = 'X';
			break;
		case COMPUTER_X:
			name = "Computer";
			symbol = 'X';
			break;
		case COMPUTER_O:
			name = "Computer";
			symbol = 'O';
			break;
		default:
			name = "Player 1";
			symbol = 'X';
			break;
	}
}

//int Player::prompt(Board board) {}

HumanPlayer::HumanPlayer(char symbol) : Player((symbol == 'X' ? HUMAN_X : HUMAN_O)) {}
int HumanPlayer::prompt(Board board, Renderer &renderer) {
	int squareNum{};
	//std::cout << name << " (" << symbol << "), enter your move (1-9): ";
	//std::cin >> squareNum;
	renderer.renderText(name + " (" + symbol + "), enter your move (1-9): ", 36, false);
	squareNum = std::stoi(renderer.prompt());
	return squareNum;
}

ComputerPlayer::ComputerPlayer(char symbol) : Player((symbol == 'X' ? COMPUTER_X : COMPUTER_O)), engine() {}
int ComputerPlayer::prompt(Board board, Renderer &renderer) {
	std::pair<int, int> move = engine.findBestMove(board);
	return utils::getSquareNum(move.first, move.second);
}