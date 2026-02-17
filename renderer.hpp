#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "board.hpp"

class Renderer {
	public:
		Renderer();
		void renderText(const std::string &text, int length = -1, bool newLine = true) const;
		void newLine() const;
		void clearScreen() const;
		void renderStartingScreen();
		void renderPlayingScreen(Board& board, std::string errorMessage, std::string promptMessage);
		void labelScreenColumns();
		void labelScreenRows();
		std::string prompt(int promptMessageLength) const;
		static void renderGameOverMessage(char winner);
	private:
		const int screenWidth;
		const int screenHeight;
		void horizontalLine(int length) const;
		void setCursorHeight(int totalContentLines) const;
		void setCursorPosition(int x, int y) const;
};

#endif
