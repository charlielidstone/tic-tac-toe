#include <iostream>
#include <regex>
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

HumanPlayer::HumanPlayer(char symbol) : Player((symbol == 'X' ? HUMAN_X : HUMAN_O)) {}
int HumanPlayer::prompt(Board board, Renderer &renderer, std::string promptMessage) {
	int squareNum{};

	std::string input = renderer.prompt(promptMessage.length());
	std::regex pattern("^[1-9]$");
	if (!std::regex_match(input, pattern)) {
		return -1;
	}
	squareNum = std::stoi(input);
	return squareNum;
}

ComputerPlayer::ComputerPlayer(char symbol) : Player((symbol == 'X' ? COMPUTER_X : COMPUTER_O)), engine() {}
int ComputerPlayer::prompt(Board board, Renderer &renderer, std::string promptMessage) {
	std::pair<int, int> move = engine.findBestMove(board);
	return utils::getSquareNum(move.first, move.second);
}