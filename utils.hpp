#ifndef UTILS_HPP
#define UTILS_HPP

#include <string>

namespace utils {
	void clearScreen();
	void log(const std::string& filename, const std::string& text, bool newLine);
	int getSquareNum(int x, int y);
	std::pair<int, int> getPair(int squareNum);
	enum UpdateStatus {
		success,
		failSpaceOccupied,
		failInvalidInput,
		failUnknown,
	};
}

#endif