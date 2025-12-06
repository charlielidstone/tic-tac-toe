/**
*  ___  _  __   ___  _   __   ___ _  ___
* |_ _|| |/ _| |_ _|/ \ / _| |_ _/ \| __|
*  | | | ( (_   | || o ( (_   | ( o ) _|
*  |_| |_|\__|  |_||_n_|\__|  |_|\_/|___|
* 
* @brief a little tic-tac-toe game in the console made with a mini-max algorithm
* @author Charlie Lidstone
*/

/*
* less than ideal scenarios:
* - when the computer goes first, and the user goes 2->5, the bot doesn't take the win
*/

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <limits>
#include <thread>
#include <chrono>
#include <fstream>

enum UpdateStatus {
    success,
    failSpaceOccupied,
    failInvalidInput,
    failUnknownError,
};

static void printBoard(char board[3][3]);
static bool isMovesLeft(char board[3][3]);
static void printExampleBoard();
static void printErrorLog(const std::vector<std::string>& errorLog);
static void clearConsole();
static std::pair<int, int> getCoordinates(int squareNum);
static int getSquareNum(int x, int y);
static UpdateStatus updateBoard(char(&board)[3][3], int squareNum, bool isP1);
static int evaluateBoard(char (&board)[3][3]);
static int minimax(char(&board)[3][3], bool isMax, int depth);
static std::pair<int, int> findBestMove(char(&board)[3][3]);
static int playGame();
static void printTitle();
static void printRepeatedChar(std::string c, int n);
static std::string center(const std::string& text, int width = 60);
static void drawFrame(const std::string& title);

static void logDebugState(std::pair<int, int> position, char board[3][3], int depth, int score);

std::ofstream log_file("log.txt");

int main() {
    printTitle();
    playGame();

    log_file.flush();

    return 0;
}



/**
 * @brief Checks if there are any empty spaces left on the board.
 * @param board 3x3 array representing the current game state.
 * @return True if at least one empty space (' ') exists, false otherwise.
 */
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

/**
 * @brief Prints an example tic-tac-toe board with numbered positions (1-9).
 */
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

/**
 * @brief Converts a square number (1-9) to its corresponding board coordinates.
 * @param squareNum Board position (1-9).
 * @return Pair (x, y) representing row and column indices.
 */
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

/**
 * @brief Converts board coordinates to the corresponding square number (1-9).
 * @param x Row index (0-2).
 * @param y Column index (0-2).
 * @return Square number corresponding to the given coordinates.
 */
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

/**
 * @brief Updates the board with the current player's move
 * @param board, Reference to the 3�3 board array containing 'X', 'O', or ' '.
 * @param squareNum, The target square number (1�9) for the move.
 * @param isP1 True for Player 1 ('X'), false for Player 2 ('O').
 * @return UpdateStatus: success, failInvalidInput, failSpaceOccupied, or failUnknownError.
 */
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

/**
* @brief determines the state of the game for the given board
* @param board, reference to 2D array representing 3x3 game board
* @return 10 if the board is a win for O, -10 if it is a win for X, 0 otherwise
*/
static int evaluateBoard(char (&board)[3][3]) {
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

/*
* @brief returns a score for a single board position by repeatedly calling evaluateBoard() until a win, draw, or loss is reached
* @param board, reference to 2D array representing the game board
* @param isMax, boolean for whether or not we should maximise the score. We want to maximise for our score, minimise for the opponents
* @param depth, an integer representing the recursive level of the function call
* @return an integer representing the score the the given position
*/
static int minimax(char(&board)[3][3], bool isMax, int depth) {
    int score = evaluateBoard(board);

    //std::cout << "depth: " << depth << std::endl;

    // we subract the depth because we want to prioritise the moves that are closest to the top of the tree
    if (score == 10) return score - depth;
    if (score == -10) return score + depth;

    if (!isMovesLeft(board)) return 0;

    if (isMax) {
        int bestScore = std::numeric_limits<int>::max();

        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (board[row][col] == ' ') {
                    board[row][col] = 'X';
                    int val = minimax(board, false, depth + 1);
                    logDebugState({ row, col }, board, depth, score);
                    board[row][col] = ' ';
                    //std::cout << "bestScore: " << bestScore << "\n";
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
                    int val = minimax(board, true, depth + 1);
                    logDebugState({ row, col }, board, depth, score);
                    board[row][col] = ' ';
                    //std::cout << "bestScore: " << bestScore << "\n";
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
                //std::cout << "\nchecking position " << row << ", " << col << "\n";
                board[row][col] = 'O';
                logDebugState({ row, col }, board, 0, score);
                score = minimax(board, true, 1);
                //std::cout << "score: " << score << "\n";
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

static int playGame() {
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
            //std::cin >> squareNum;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            auto bestMove = findBestMove(board);
            squareNum = getSquareNum(bestMove.first, bestMove.second);
            //std::cout << "\nChosen square: " << squareNum;
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            //std::cout << "\nbestMove: " << bestMove.first << ", " << bestMove.second;
        }
        else if (currentPlayer == "Player 2 (O's)") {
            std::cin >> squareNum;
            //std::this_thread::sleep_for(std::chrono::milliseconds(500));
            //auto bestMove = findBestMove(board);
            //squareNum = getSquareNum(bestMove.first, bestMove.second);
            ////std::cout << "\nChosen square: " << squareNum;
            //std::this_thread::sleep_for(std::chrono::milliseconds(500));
            ////std::cout << "\nbestMove: " << bestMove.first << ", " << bestMove.second;
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
    }
    else if (evaluateBoard(board) == -10) {
        std::cout << "\nGood game! Player 2 wins!" << std::endl;
    }
    else if (evaluateBoard(board) == 0) {
        std::cout << "\nGood game! It's a TIE!" << std::endl;
    }
    else {
        std::cout << "\nGood game!" << std::endl;
    }

    return 0;
}

/**
 * @brief prints the ascii title
 */
static void printTitle() {
    printf("\n ___  _  __   ___  _   __   ___ _  ___\n|_ _|| |/ _| |_ _|/ \\ / _| |_ _/ \\| __|\n | | | ( (_   | || o ( (_   | ( o ) _|\n |_| |_|\\__|  |_||_n_|\\__|  |_|\\_/|___|\n");
}

static void playingScreen() {

}

static void printHeading(std::string text, std::string borderChar, int marginRight) {
    printRepeatedChar(" ", marginRight);
    printRepeatedChar(borderChar, text.length());
    std::cout << text;
}

static void printRepeatedChar(std::string c, int n) {
    for (int i = 0; i < n; i++) {
        std::cout << c;
    }
}

static void printMenuScreen() {
    printHeading("Tic Tac Toe", "-", 2);
}

static void logDebugState(std::pair<int, int> position, char board[3][3], int depth, int score) {
    if (depth == 0) {
        log_file << "-------------------------------------------------------------------\n";
    }
    for (int row = 0; row < 3; row++) {
        log_file << " " << board[row][0] << " | "
            << board[row][1]
            << " | " << board[row][2];
        if (row == 0) {
            log_file << "   [" << position.first << ", " << position.second << "] " << "depth: " << depth << ", score: " << score << "\n";
        }
        else {
            log_file << " \n";
        }
        if (row < 2) {
            log_file << "---+---+---\n";
        }
        else {
            log_file << "\n";
        }
    }
}

static void drawFrame(const std::string& title) {
    const int width = 60;

    std::cout << "+" << std::string(width, '-') << "+\n";
    std::cout << "|" << center(title, width) << "|\n";
    std::cout << "+" << std::string(width, '-') << "+\n\n";
}

static std::string center(const std::string& text, int width = 60) {
    int pad = (width - text.size()) / 2;
    if (pad < 0) pad = 0;
    return std::string(pad, ' ') + text;
}
