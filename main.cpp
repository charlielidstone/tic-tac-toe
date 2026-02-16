#include <iostream>
#include <chrono>
#include <thread>
#include "game.hpp"

int main(void) {
	Game game(false);
	game.start();

	return 0;
}