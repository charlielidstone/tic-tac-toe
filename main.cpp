#include <iostream>
#include <chrono>
#include <thread>
#include "game.hpp"

int main(void) {

	Renderer renderer;
	renderer.renderStartingScreen();
	std::cin.get();

	Game game(true, renderer);
	game.displayStartingScreen();

	return 0;
}