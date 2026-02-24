#include <iostream>
#include <fstream>
#include "utils.hpp"

namespace utils {

	int getSquareNum(int x, int y) {
		return x * 3 + y + 1;
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

    void log(const std::string& filename, const std::string& text, bool newLine = true) {
        std::ofstream log_file(filename, std::ios::app);

        if (!log_file) {
            std::cerr << "Failed to open log file for writing: " << filename << std::endl;
            return;
        }

        log_file << text;
        if (newLine) {
            log_file << '\n';
        }
    }
}