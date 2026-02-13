# Fix for Coordinate Conversion Bug in breaking-it-up Branch

## Problem Description

The bot in the `breaking-it-up` branch exhibits critical failures:
- Sometimes chooses squares that are already occupied
- Sometimes loses games (should never happen with minimax)
- Makes poor strategic moves

## Root Cause

The `getSquareNum()` function in `utils.cpp` uses an incorrect formula that doesn't match the coordinate system used by `getPair()`.

### The Bug

**File:** `utils.cpp`, line 8-10

```cpp
// BUGGY CODE - DO NOT USE
int getSquareNum(int x, int y) {
    return y * 3 + x + 1;  // WRONG! Treats y as row, x as column
}
```

**The issue:** This formula assumes `y` is the row and `x` is the column, but `getPair()` (the inverse function) returns coordinates where `x` is the row and `y` is the column. This mismatch causes the round-trip conversion to fail for most board positions.

### Verification Test Results

Testing the buggy code shows that round-trip conversion only works for the diagonal (squares 1, 5, 9):

```
Square 1 -> getPair(1) -> (0,0) -> getSquareNum(0,0) -> 1 ✓
Square 2 -> getPair(2) -> (0,1) -> getSquareNum(0,1) -> 4 ✗ WRONG!
Square 3 -> getPair(3) -> (0,2) -> getSquareNum(0,2) -> 7 ✗ WRONG!
Square 4 -> getPair(4) -> (1,0) -> getSquareNum(1,0) -> 2 ✗ WRONG!
Square 5 -> getPair(5) -> (1,1) -> getSquareNum(1,1) -> 5 ✓
Square 6 -> getPair(6) -> (1,2) -> getSquareNum(1,2) -> 8 ✗ WRONG!
Square 7 -> getPair(7) -> (2,0) -> getSquareNum(2,0) -> 3 ✗ WRONG!
Square 8 -> getPair(8) -> (2,1) -> getSquareNum(2,1) -> 6 ✗ WRONG!
Square 9 -> getPair(9) -> (2,2) -> getSquareNum(2,2) -> 9 ✓
```

### Impact

When the AI's minimax algorithm determines the best move is at board position (row, col), it calls `getSquareNum(row, col)` to convert to a square number. Due to the bug, the square number is often wrong, causing:

1. **Wrong square selection:** AI tries to place its move at the wrong position
2. **Occupied square errors:** The wrong position might already be occupied
3. **Missed wins:** AI calculates the winning move but places it incorrectly
4. **Strategic failures:** AI's entire decision-making is based on wrong board positions

## The Fix

Replace the buggy formula with the correct logic from the `master` branch:

**File:** `utils.cpp`, lines 8-10

```cpp
// FIXED CODE - Matches master branch and getPair()
int getSquareNum(int x, int y) {
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

### Why This Works

This implementation correctly interprets:
- `x` as the row index (0=top, 1=middle, 2=bottom)
- `y` as the column index (0=left, 1=middle, 2=right)

Which matches the board layout:
```
 1 | 2 | 3     (x=0, y=0,1,2)
---+---+---
 4 | 5 | 6     (x=1, y=0,1,2)
---+---+---
 7 | 8 | 9     (x=2, y=0,1,2)
```

### Verification After Fix

All round-trips now work correctly:

```
Square 1 -> getPair(1) -> (0,0) -> getSquareNum(0,0) -> 1 ✓
Square 2 -> getPair(2) -> (0,1) -> getSquareNum(0,1) -> 2 ✓
Square 3 -> getPair(3) -> (0,2) -> getSquareNum(0,2) -> 3 ✓
Square 4 -> getPair(4) -> (1,0) -> getSquareNum(1,0) -> 4 ✓
Square 5 -> getPair(5) -> (1,1) -> getSquareNum(1,1) -> 5 ✓
Square 6 -> getPair(6) -> (1,2) -> getSquareNum(1,2) -> 6 ✓
Square 7 -> getPair(7) -> (2,0) -> getSquareNum(2,0) -> 7 ✓
Square 8 -> getPair(8) -> (2,1) -> getSquareNum(2,1) -> 8 ✓
Square 9 -> getPair(9) -> (2,2) -> getSquareNum(2,2) -> 9 ✓
```

## Additional Fix Required

**File:** `game.hpp`, line 4

The header file is missing an include directive needed for `std::unique_ptr`:

```cpp
#ifndef GAME_HPP
#define GAME_HPP

#include <memory>  // ADD THIS LINE
#include "player.hpp"
```

Without this, the code won't compile with some C++ compilers.

## Testing

After applying both fixes:

1. **Build the game:**
   ```bash
   g++ -std=c++17 -o tic-tac-toe main.cpp game.cpp board.cpp engine.cpp player.cpp utils.cpp
   ```

2. **Test gameplay:**
   The bot should now:
   - Never choose occupied squares
   - Never lose (always win or tie)
   - Make optimal moves according to minimax algorithm

3. **Manual test:** Play a few games against the bot to verify it plays correctly.

## Applying the Fixes

### Option 1: Manual Edit

Edit the two files directly:
1. `utils.cpp`: Replace the `getSquareNum()` function
2. `game.hpp`: Add `#include <memory>` after line 2

### Option 2: Using Git Patch

A patch file is provided: `coordinate_bug_fix.patch`

Apply it with:
```bash
git apply coordinate_bug_fix.patch
```

## Conclusion

This bug was a critical flaw that made the `breaking-it-up` branch unusable despite its superior architecture. The bug was caused by a simple coordinate system mismatch - using a mathematical formula that transposed x and y compared to what the rest of the codebase expected.

The fix restores the bot to full functionality while maintaining all the architectural improvements of the refactored branch.
