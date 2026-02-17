#include <iostream>
#include <chrono>
#include <thread>
#include "game.hpp"

int main(void) {

	Game game(true);
	game.displayStartingScreen();

	return 0;
}