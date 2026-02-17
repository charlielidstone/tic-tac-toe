#include "renderer.hpp"
#include <iostream>

Renderer::Renderer() : screenWidth(120), screenHeight(30) {}

void Renderer::clearScreen() const {
	std::cout << "\033[H\033[J";
}

void Renderer::setCursorHeight(int totalContentLines) const {
	std::cout << "\033[" << (screenHeight / 2 - totalContentLines) << "B";
}

std::string Renderer::prompt() const {
	std::string value{};
	std::cin >> value;
	return value;
}

void Renderer::renderPlayingScreen(Board &board) {
	const int TOTAL_LINES = 10;
	clearScreen();
	horizontalLine(screenWidth);
	setCursorHeight(TOTAL_LINES);
	std::string title = "Tic Tac Toe\n\n";
	renderText(title);

	const std::string grey = "\033[38;2;80;80;80m";
	const std::string reset = "\033[0m";

	std::string rowText = "";

	for (int row = 0; row < 3; row++) {
		for (int col = 0; col < 3; col++) {
			if (board.getCell(row, col) == ' ') {
				rowText += " " + grey + std::to_string(row * 3 + col + 1) + reset;
				if (col < 2) {
					rowText += " |";
				}
			}
			else {
				rowText += " ";
				rowText += board.getCell(row, col);
				if (col < 2) {
					rowText += " |";
				}
			}
		}
		renderText(rowText, 12);
		rowText = "";
		if (row < 2) {
			renderText("---+---+---", 12);
		}
	}

}

void Renderer::horizontalLine(int length) const {
	std::cout << " ";
	for (int i = 0; i < length-1; i++) {
		std::cout << "-";
	}
	std::cout << std::endl;
}

void Renderer::renderStartingScreen() {
	const int TOTAL_LINES = 4;
	clearScreen();
	setCursorHeight(TOTAL_LINES);
	std::string welcomeMessage = "Welcome to Tic Tac Toe!\n\n";
	std::string instructions = "Press Enter to start a new game.";
	renderText(welcomeMessage);
	renderText(instructions);
}

void Renderer::renderText(const std::string &text, const int length, bool newLine) const {
	std::cout
		<< "\033[" << (screenWidth / 2 - (length == -1 ? text.length() : length)/ 2) << "C"
		<< text;
	if (newLine) {
		std::cout << std::endl;
	}
}

void Renderer::labelScreenColumns() {
	for (int i = 1; i <= 200; i++) {
		if (i % 10 == 0) {
			std::cout << i;
			if (i < 100) {
				i++;
			}
			else {
				i += 2;
			}
		}
		else {
			std::cout << " ";
		}
	}
	std::cout << std::endl;
}

void Renderer::labelScreenRows() {
	for (int i = 1; i <= 50; i++) {
		std::cout << "\033[" << (screenHeight/2 - i) << "B" << i << std::endl;
	}
}