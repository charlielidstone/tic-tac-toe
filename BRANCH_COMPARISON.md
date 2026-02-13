# Game Logic Comparison: master vs breaking-it-up

## Executive Summary

The `breaking-it-up` branch represents a significant refactoring of the `master` branch code, transforming a monolithic single-file implementation into a well-structured, object-oriented architecture. **The core game logic (minimax algorithm and board evaluation) remains functionally equivalent between both branches**, but the refactored version provides better maintainability, testability, and extensibility.

## Architectural Differences

### Master Branch Structure
- **Single file**: All code in `main.cpp` (~445 lines)
- **Procedural approach**: Functions operate on raw arrays
- **Global state**: Log file handle at file scope
- **Tight coupling**: Game logic, UI, and AI tightly interwoven

### Breaking-it-up Branch Structure
- **Multi-file modular design**: Code split into logical components
  - `main.cpp`: Entry point (9 lines)
  - `game.cpp/hpp`: Game flow and state management
  - `board.cpp/hpp`: Board representation and manipulation
  - `engine.cpp/hpp`: AI minimax algorithm
  - `player.cpp/hpp`: Player abstraction (Human/Computer)
  - `utils.cpp/hpp`: Utility functions
- **Object-oriented approach**: Classes with clear responsibilities
- **Better encapsulation**: Data and methods grouped logically
- **Loose coupling**: Clear separation of concerns

## Core Game Logic Comparison

### 1. Board Evaluation (`evaluateBoard` / `Board::evaluate`)

**Functionally Identical** - Both implementations:
- Check all 3 rows for wins
- Check all 3 columns for wins
- Check both diagonals for wins
- Return +10 for 'O' win, -10 for 'X' win, 0 for no win

**Master Branch** (lines 186-213):
```cpp
static int evaluateBoard(char (&board)[3][3]) {
    // Check rows
    for (int row = 0; row < 3; row++) {
        if (board[row][0] != ' ' &&
            board[row][0] == board[row][1] &&
            board[row][1] == board[row][2]) {
            return board[row][0] == 'O' ? +10 : -10;
        }
    }
    // Check columns
    for (int col = 0; col < 3; col++) {
        if (board[0][col] != ' ' &&
            board[0][col] == board[1][col] &&
            board[1][col] == board[2][col]) {
            return board[0][col] == 'O' ? +10 : -10;
        }
    }
    // Check diagonals...
    return 0;
}
```

**Breaking-it-up Branch** (`board.cpp`, lines 90-117):
```cpp
int Board::evaluate() const {
    // Check rows
    for (int row = 0; row < 3; row++) {
        if (getCell(row, 0) != ' ' &&
            getCell(row, 0) == getCell(row, 1) &&
            getCell(row, 1) == getCell(row, 2)) {
            return getCell(row, 0) == 'O' ? +10 : -10;
        }
    }
    // Check columns
    for (int col = 0; col < 3; col++) {
        if (getCell(0, col) != ' ' &&
            getCell(0, col) == getCell(1, col) &&
            getCell(1, col) == getCell(2, col)) {
            return getCell(0, col) == 'O' ? +10 : -10;
        }
    }
    // Check diagonals...
    return 0;
}
```

**Difference**: Refactored version uses `getCell()` accessor instead of direct array access, providing better encapsulation.

### 2. Minimax Algorithm (`minimax` function)

**Functionally Identical** - Both implementations:
- Use depth-adjusted scoring (prioritize faster wins)
- Implement the same minimax logic with alpha-beta principles
- Use 'X' as minimizing player, 'O' as maximizing player
- Return depth-adjusted scores (+10 - depth for O wins, -10 + depth for X wins)

**Master Branch** (lines 222-271):
```cpp
static int minimax(char(&board)[3][3], bool isMax, int depth) {
    int score = evaluateBoard(board);
    
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
                    logDebugState({ row, col }, board, depth, score); // logging
                    board[row][col] = ' ';
                    if (val <= bestScore) {
                        bestScore = val;
                    }
                }
            }
        }
        return bestScore;
    }
    // else maximize...
}
```

**Breaking-it-up Branch** (`engine.cpp`, lines 9-52):
```cpp
int Engine::minimax(Board board, bool isMax, int depth) {
    int score = board.evaluate();
    
    if (score == 10) return score - depth;
    if (score == -10) return score + depth;
    
    if (!board.isMovesLeft()) return 0;
    
    if (isMax) {
        int bestScore = std::numeric_limits<int>::max();
        for (int row = 0; row < 3; row++) {
            for (int col = 0; col < 3; col++) {
                if (board.getCell(row, col) == ' ') {
                    board.setCell(row, col, 'X');
                    int val = minimax(board, false, depth + 1);
                    board.setCell(row, col, ' ');
                    if (val <= bestScore) {
                        bestScore = val;
                    }
                }
            }
        }
        return bestScore;
    }
    // else maximize...
}
```

**Key Differences**:
1. **Board passing**: Master uses reference to raw array; breaking-it-up passes Board object by value (making a copy for each recursive call)
2. **Cell access**: Refactored version uses `getCell()` and `setCell()` methods
3. **Debug logging**: Master includes debug logging calls; refactored version removed them

### 3. Best Move Selection (`findBestMove`)

**Functionally Identical** - Both implementations:
- Iterate through all empty cells
- Simulate 'O' moves (computer player)
- Call minimax starting with depth 1
- Select move with highest score
- Return coordinates of best move

**Master Branch** (lines 273-296):
```cpp
static std::pair<int, int> findBestMove(char(&board)[3][3]) {
    std::pair<int, int> bestMove = { -1, -1 };
    int bestScore = std::numeric_limits<int>::min();
    int score{};

    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (board[row][col] == ' ') {
                board[row][col] = 'O';
                logDebugState({ row, col }, board, 0, score);
                score = minimax(board, true, 1);
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
```

**Breaking-it-up Branch** (`engine.cpp`, lines 54-74):
```cpp
std::pair<int, int> Engine::findBestMove(Board board) {
    std::chrono::milliseconds time(artificialDelay);
    std::this_thread::sleep_for(time);
    
    std::pair<int, int> bestMove = { -1, -1 };
    int bestScore = std::numeric_limits<int>::min();
    int score{};
    
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 3; col++) {
            if (board.getCell(row, col) == ' ') {
                board.setCell(row, col, 'O');
                score = minimax(board, true, 1);
                board.setCell(row, col, ' ');
                if (score >= bestScore) {
                    bestScore = score;
                    bestMove = { row, col };
                }
            }
        }
    }
    return bestMove;
}
```

**Key Differences**:
1. **Artificial delay**: Refactored version includes a 2-second delay at the start (configurable via `artificialDelay` member variable)
2. **Debug logging**: Master version includes logging; refactored removed it

### 4. Coordinate Conversion

**Functionally Different (Bug Fix!)**

**Master Branch** (`getSquareNum`, lines 136-148):
```cpp
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
```

**Breaking-it-up Branch** (`utils.cpp`, lines 8-10):
```cpp
int getSquareNum(int x, int y) {
    return y * 3 + x + 1;
}
```

**Analysis**: The refactored version uses a mathematical formula that is more concise and less error-prone. Both should produce the same results, but the refactored version is cleaner.

**Master Branch** (`getCoordinates`, lines 116-128) and **Breaking-it-up** (`getPair`, lines 11-23) are functionally identical but with different names.

## Game Flow Differences

### Master Branch
- **Hardcoded roles**: Player 1 is computer (X), Player 2 is human (O)
- **Direct control flow**: Uses do-while loop in `playGame()` function
- **Player switching**: Simple string comparison
- **Error handling**: In-place with if-else chains

### Breaking-it-up Branch
- **Flexible player types**: Constructor parameter determines if bot starts and player types
- **Object-oriented flow**: Game class manages state, delegates to Player objects
- **Polymorphic players**: HumanPlayer and ComputerPlayer both inherit from Player base class
- **Player switching**: Clean pointer swapping between player1 and player2
- **Error handling**: Delegated to Board::handleError method

### Player Assignment Bug Fix

**Master Branch** (lines 366-376):
```cpp
if (evaluateBoard(board) == +10) {
    std::cout << "\nGood game! Player 1 wins!" << std::endl;
}
else if (evaluateBoard(board) == -10) {
    std::cout << "\nGood game! Player 2 wins!" << std::endl;
}
```

This is **INCORRECT** in master! Since 'O' = +10 and 'X' = -10, and Player 1 is X while Player 2 is O, the win messages are backwards!

**Breaking-it-up Branch** (game.cpp, lines 63-68):
```cpp
if (board.evaluate() == +10) {
    std::cout << "\nGood game! Player 2 wins!" << std::endl;
}
else if (board.evaluate() == -10) {
    std::cout << "\nGood game! Player 1 wins!" << std::endl;
}
```

This is **CORRECT**! The refactored branch properly associates +10 with Player 2 (O) and -10 with Player 1 (X).

## Notable Commented Code

### Master Branch Issues
- Lines 321-337: Significant portions of player logic are commented out, creating confusion about which player is human vs computer
- Lines 389-407: Several unused/incomplete utility functions (playingScreen, printHeading, printMenuScreen)
- Lines 409-430: Debug logging function that's called but may not be needed

## Summary of Differences

### ✅ Identical Core Logic
- **Minimax algorithm**: Same logic, depth handling, and scoring
- **Board evaluation**: Same win detection for rows, columns, diagonals
- **Best move selection**: Same approach to finding optimal moves

### 🔧 Improvements in breaking-it-up
- **Object-oriented design**: Better separation of concerns
- **Modularity**: Code split into logical files
- **Extensibility**: Easy to add new player types or board sizes
- **Win message bug fix**: Corrected player win announcements
- **Cleaner coordinate conversion**: Mathematical formula instead of if-chains
- **Removed commented code**: Cleaner, production-ready codebase
- **Player flexibility**: Can configure human vs computer easily
- **Better encapsulation**: Board state properly protected
- **Artificial delay**: Configurable thinking time for AI

### ⚠️ Potential Concerns in breaking-it-up
- **Board copying**: Minimax passes Board by value (copies entire board each recursion). Master passed by reference. This could impact performance slightly, though for a 3x3 board it's negligible.
- **Removed debug logging**: No more detailed debug output to log.txt

## Recommendations

1. **For new development**: Use the `breaking-it-up` branch as the base. The architecture is significantly better.

2. **Win condition bug**: The `master` branch has the win messages reversed. This is fixed in `breaking-it-up`.

3. **Performance**: For a 3x3 tic-tac-toe game, performance differences are negligible. The architectural benefits of `breaking-it-up` far outweigh any minor performance considerations.

4. **Debug logging**: If detailed debug logging is needed, consider adding it back to the Engine class in `breaking-it-up` as an optional feature.

5. **Code quality**: The `breaking-it-up` branch is more maintainable, testable, and follows better software engineering practices.

## Conclusion

**The game logic algorithms are functionally equivalent between both branches.** The minimax algorithm, board evaluation, and move selection all work the same way. The primary differences are:

1. **Architecture**: breaking-it-up uses OOP and modular design vs monolithic procedural code
2. **Bug fix**: Win condition messages are corrected in breaking-it-up
3. **Code quality**: breaking-it-up removes commented code and provides better structure
4. **Extensibility**: breaking-it-up makes it much easier to extend functionality

The `breaking-it-up` branch represents a superior implementation that maintains the same game logic while providing a much better foundation for future development.
