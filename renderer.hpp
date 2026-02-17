#ifndef RENDERER_HPP
#define RENDERER_HPP

#include "board.hpp"

class Renderer {
	public:
		Renderer();
		void renderText(const std::string &text, int length = -1, bool newLine = true) const;
		void clearScreen() const;
		void renderStartingScreen();
		void renderPlayingScreen(Board& board);
		void labelScreenColumns();
		void labelScreenRows();
		std::string prompt() const;
		static void renderGameOverMessage(char winner);
	private:
		const int screenWidth;
		const int screenHeight;
		void horizontalLine(int length) const;
		void setCursorHeight(int totalContentLines) const;
};

#endif
