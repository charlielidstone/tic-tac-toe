#include <iostream>
#include "utils.hpp"

namespace utils {
	void clearScreen() {
		std::cout << "\033[2J\033[1;1H";
	}
	int getSquareNum(int x, int y) {
		return y * 3 + x + 1;
	}
    std::pair<int, int> getPair(int squareNum) {
        int x{};
        if (squareNum < 10 && squareNum > 6) x = 2;
        if (squareNum < 7 && squareNum > 3) x = 1;
        if (squareNum < 4 && squareNum > 0) x = 0;

        int y{};
        if (squareNum % 3 == 0) y = 2;
        if (squareNum % 3 == 2) y = 1;
        if (squareNum % 3 == 1) y = 0;

        return { x, y };
    }
}