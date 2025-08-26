#include <iostream>
#include <string>
#include <vector>
#include <regex>

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

static UpdateStatus updateBoard(char(&board)[3][3], int squareNum, bool isP1) {
    std::regex pattern(R"(^[1-9]$)");
    std::string squareNumStr = std::to_string(squareNum);
    if (!std::regex_search(squareNumStr, pattern))
        return failInvalidInput;

    int x{};
    if (squareNum < 10 && squareNum > 6) x = 2;
    if (squareNum < 7 && squareNum > 3) x = 1;
    if (squareNum < 4 && squareNum > 0) x = 0;

    int y{};
    if (squareNum % 3 == 0) y = 2;
    if (squareNum % 3 == 2) y = 1;
    if (squareNum % 3 == 1) y = 0;

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
            return board[row][0] == 'X' ? +1 : -1;
        }
    }
    for (int col = 0; col < 3; col++) {
        if (board[0][col] != ' ' &&
            board[0][col] == board[1][col] &&
            board[1][col] == board[2][col]) {
            return board[0][col] == 'X' ? +1 : -1;
        }
    }
    if (board[0][0] != ' ' &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2]) {
        return board[0][0] == 'X' ? +1 : -1;
    }
    if (board[0][2] != ' ' &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0]) {
        return board[0][2] == 'X' ? +1 : -1;
    }

    return 0;
}

int	main() {
    bool isGameOver{ false };

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
        std::cin >> squareNum;

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

        isGameOver = evaluateBoard(board) || !isMovesLeft(board); // evaluateBoard() returns 0, +1, -1, for no win, X win, O win, respectively

    } while (!isGameOver);

    clearConsole();
    printExampleBoard();
    printBoard(board);
    if (evaluateBoard(board) == +1) {
        std::cout << "\nGood game! Player 1 wins!" << std::endl;
    } else if (evaluateBoard(board) == -1) {
        std::cout << "\nGood game! Player 2 wins!" << std::endl;
    } else if (evaluateBoard(board) == 0) {
        std::cout << "\nGood game! It's a TIE!" << std::endl;
    }
    else {
        std::cout << "\nGood game!" << std::endl;
    }

    return 0;
}