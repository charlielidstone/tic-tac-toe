#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <limits>
#include <thread>
#include <chrono>

static void printBoard(char board[3][3]) {
    for (int row = 0; row < 3; row++) {
        std::cout << " " << board[row][0] << " | "
            << board[row][1]
            << " | " << board[row][2] << " \n";
        if (row < 2) {
            std::cout << "---+---+---\n";
        }
    }
}

static bool isMovesLeft(char board[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                return true;
            }
        }
    }
    return false;
}

static void printExampleBoard() {
    std::cout << " 1 | 2 | 3\n---+---+---\n 4 | 5 | 6\n---+---+--\n 7 | 8 | 9 \n\n";
}

static void printErrorLog(const std::vector<std::string>& errorLog) {
    std::cout << "\n";
    for (const std::string& error : errorLog) {
        std::cout << error << "\n";
    }
}

static void clearConsole() {
    std::cout << "\033[2J\033[1;1H";
}

enum UpdateStatus {
    success,
    failSpaceOccupied,
    failInvalidInput,
    failUnknownError,
};

static std::pair<int, int> getCoordinates(int squareNum) {
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

static int getSquareNum(int x, int y) {
    int squareNum{};

    if (x == 2) squareNum = 7;
    if (x == 1) squareNum = 4;
    if (x == 0) squareNum = 1;

    if (y == 0) squareNum += 0;
    if (y == 1) squareNum += 1;
    if (y == 2) squareNum += 2;

    return squareNum;
}


static UpdateStatus updateBoard(char(&board)[3][3], int squareNum, bool isP1) {
    std::regex pattern(R"(^[1-9]$)");
    std::string squareNumStr = std::to_string(squareNum);
    if (!std::regex_search(squareNumStr, pattern))
        return failInvalidInput;

    auto moveCoordinates = getCoordinates(squareNum);
    int x = moveCoordinates.first;
    int y = moveCoordinates.second;

    if (isP1 && board[x][y] == ' ') {
        board[x][y] = 'X';
        return success;
    }
    else if (!isP1 && board[x][y] == ' ') {
        board[x][y] = 'O';
        return success;
    }
    else if (board[x][y] != ' ')
        return failSpaceOccupied;
    else
        return failUnknownError;
}

static int evaluateBoard(char(&board)[3][3]) {
    static char referenceMark{ board[0][0] };
    for (int row = 0; row < 3; row++) {
        if (board[row][0] != ' ' &&
            board[row][0] == board[row][1] &&
            board[row][1] == board[row][2]) {
            return board[row][0] == 'O' ? +10 : -10;
        }
    }
    for (int col = 0; col < 3; col++) {
        if (board[0][col] != ' ' &&
            board[0][col] == board[1][col] &&
            board[1][col] == board[2][col]) {
            return board[0][col] == 'O' ? +10 : -10;
        }
    }
    if (board[0][0] != ' ' &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2]) {
        return board[0][0] == 'O' ? +10 : -10;
    }
    if (board[0][2] != ' ' &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0]) {
        return board[0][2] == 'O' ? +10 : -10;
    }

    return 0;
}

// minimax() returns a score for a single board position. it goes through every possible position after the given position by calling itself until a win, loss, or tie is reached.
static int minimax(char(&board)[3][3], bool isMax, int depth) {
    int score = evaluateBoard(board);

    if (score == 10) return score - depth;
    if (score == -10) return score + depth;

    if (!isMovesLeft(board)) return 0;

    if (isMax) {
        int bestScore = std::numeric_limits<int>::max();;

        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (board[row][col] == ' ') {
                    board[row][col] = 'X';
                    int val = minimax(board, true, depth + 1);
                    board[row][col] = ' ';
                    if (val <= bestScore) {
                        bestScore = val;
                    }
                }
            }
        }
        return bestScore;
    }
    else {
        int bestScore = std::numeric_limits<int>::min();

        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (board[row][col] == ' ') {
                    board[row][col] = 'O';
                    int val = minimax(board, false, depth + 1);
                    board[row][col] = ' ';
                    if (val >= bestScore) {
                        bestScore = val;
                    }
                }
            }
        }
        return bestScore;
    }
}

static std::pair<int, int> findBestMove(char(&board)[3][3]) {
    std::pair<int, int> bestMove = { -1, -1 };
    int bestScore = std::numeric_limits<int>::min();
    int score{};

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (board[row][col] == ' ') {
                board[row][col] = 'O';
                score = minimax(board, true, 0);
                std::cout << "\nscore: " << score;
                board[row][col] = ' ';
                if (score >= bestScore) {
                    bestScore = score;
                    bestMove = { row, col };
                }
            }
        }
    }

    return bestMove;
}

int	main() {
    bool isGameOver{ false };

    // board[row][col]
    char board[3][3] = {
        { ' ', ' ', ' ' },
        { ' ', ' ', ' ' },
        { ' ', ' ', ' ' }
    };

    std::string errorMessage{ "" };

    do {
        static std::string currentPlayer{ "Player 1 (X's)" };
        static int squareNum{};

        clearConsole();
        printExampleBoard();
        printBoard(board);
        std::cout << errorMessage << std::endl;

        std::cout << "\n" << currentPlayer << ", where would you like to go ? : ";

        if (currentPlayer == "Player 1 (X's)") {
            std::cin >> squareNum;
        }
        else if (currentPlayer == "Player 2 (O's)") {
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            auto bestMove = findBestMove(board);
            squareNum = getSquareNum(bestMove.first, bestMove.second);
            std::cout << "\nChosen square: " << squareNum;
            //std::cout << squareNum;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            std::cout << "\nbestMove: " << bestMove.first << ", " << bestMove.second;
        }

        static UpdateStatus updateStatus{ success };
        updateStatus = updateBoard(board, squareNum, (currentPlayer == "Player 1 (X's)"));
        if (updateStatus == success) {
            errorMessage = "";
            if (currentPlayer == "Player 1 (X's)")
                currentPlayer = "Player 2 (O's)";
            else
                currentPlayer = "Player 1 (X's)";
        }
        else if (updateStatus == failSpaceOccupied) {
            errorMessage = "\n" + std::to_string(squareNum) + " is already taken!";
        }
        else if (updateStatus == failInvalidInput) {
            errorMessage = "\nPlease enter a single digit from 1 to 9.";
        }
        else {
            errorMessage = "\nSome unknown error occurred. :(";
        }

        isGameOver = evaluateBoard(board) || !isMovesLeft(board); // evaluateBoard() returns 0, +10, -10, for no win, X win, O win, respectively

    } while (!isGameOver);

    clearConsole();
    printExampleBoard();
    printBoard(board);
    if (evaluateBoard(board) == +10) {
        std::cout << "\nGood game! Player 1 wins!" << std::endl;
    } else if (evaluateBoard(board) == -10) {
        std::cout << "\nGood game! Player 2 wins!" << std::endl;
    } else if (evaluateBoard(board) == 0) {
        std::cout << "\nGood game! It's a TIE!" << std::endl;
    }
    else {
        std::cout << "\nGood game!" << std::endl;
    }

    return 0;
}