#ifndef UTILS_HPP
#define UTILS_HPP

namespace utils {
	void clearScreen();
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