#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "board.hpp"

class Renderer {
	public:
		Renderer();
		void renderText(const std::string &text, int length = -1, bool newLine = true) const;
		void renderTextLeft(const std::string& text, int marginLeft = 0, bool newLine = true) const;
		void newLine() const;
		void clearScreen() const;
		void renderStartingScreen();
		void renderPlayingScreen(Board& board, std::string errorMessage, std::string promptMessage);
		void renderGameOverScreen(Board& board, char winner);
		void labelScreenColumns();
		void labelScreenRows();
		std::string prompt(int promptMessageLength) const;
		static void renderGameOverMessage(char winner);
		void borderedText(std::string text, int paddingY, int paddingX, char borderCharY, char borderCharX);
	private:
		const int screenWidth;
		const int screenHeight;
		void horizontalLine(int length) const;
		void setCursorHeight(int totalContentLines) const;
		void setCursorPosition(int x, int y) const;
};

#endif
