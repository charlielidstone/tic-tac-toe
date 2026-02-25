#ifndef GAME_HPP
#define GAME_HPP

#include "player.hpp"
#include "renderer.hpp"

class Game {
	public:
		Game(Renderer& renderer);
		Game(bool botStarts, Renderer& renderer);
		void displayStartingScreen();
		void start(Renderer &renderer);
	private:
		bool isOver;
		bool isBotGame;
		bool botStarts;
		std::unique_ptr<Player> player1;
		std::unique_ptr<Player> player2;
		Player* currentPlayer;
		Renderer& renderer;
};

#endif