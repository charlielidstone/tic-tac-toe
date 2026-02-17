#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>
#include "board.hpp"
#include "engine.hpp"
#include "renderer.hpp"

class Engine;

enum PlayerType {
	HUMAN_X,
	HUMAN_O,
	COMPUTER_X,
	COMPUTER_O,
};

class Player {
	public:
		Player();
		Player(PlayerType playertype);
		virtual ~Player() = default;
		virtual int prompt(Board board, Renderer& renderer, std::string promptMessage) = 0;
		std::string getName() const { return name; }
		virtual bool isComputer() const { return false; }
		char getSymbol() const { return symbol; }
	protected:
		PlayerType type;
		std::string name;
		char symbol;
};

class HumanPlayer : public Player {
	public:
		HumanPlayer(char symbol = 'X');
		int prompt(Board board, Renderer &renderer, std::string promptMessage) override;

};

class ComputerPlayer : public Player {
	public:
		ComputerPlayer(char symbol = 'O');
		int prompt(Board board, Renderer& renderer, std::string promptMessage) override;
		bool isComputer() const override { return true; }
	private:
		Engine engine;

};

#endif