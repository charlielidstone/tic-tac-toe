#include <iostream>
#include <chrono>
#include <thread>
#include "game.hpp"

int main(void) {

	Game game(false);
	game.displayStartingScreen();

	// Keep running until 'q' is pressed
	std::cout << "Press 'q' and Enter to exit..." << std::endl;
	char input;
	while (std::cin >> input) {
	    if (input == 'q' || input == 'Q') {
	        break;
	    }
	    std::cout << "Press 'q' and Enter to exit..." << std::endl;
	}

	return 0;
}