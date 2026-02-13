# Tic-Tac-Toe Breaking-It-Up Branch - Bug Fix Summary

## Quick Start

If you're experiencing issues with the bot in the `breaking-it-up` branch (bot choosing occupied squares, losing games, or making poor moves), apply this fix:

```bash
cd /path/to/tic-tac-toe
git checkout breaking-it-up
git apply coordinate_bug_fix.patch
g++ -std=c++17 -o tic-tac-toe main.cpp game.cpp board.cpp engine.cpp player.cpp utils.cpp
./tic-tac-toe
```

## What Was Wrong?

The `getSquareNum()` function in `utils.cpp` had a coordinate system mismatch:
- Used formula: `y * 3 + x + 1` (treats y as row, x as column)
- Expected by rest of code: x as row, y as column
- Result: AI placed moves at wrong board positions

## Files in This Fix Package

1. **FIX_COORDINATE_BUG.md** - Detailed technical analysis of the bug, testing results, and fix explanation
2. **coordinate_bug_fix.patch** - Git patch file that can be applied to fix the bug
3. **BRANCH_COMPARISON.md** - Updated comparison document noting the bug and fix
4. **README_FIX.md** - This file (quick reference)

## Verification

After applying the fix, all 9 board positions should convert correctly in round-trip testing. The bot should:
- ✓ Never choose occupied squares
- ✓ Never lose (only win or tie)
- ✓ Make optimal moves using minimax algorithm

## Questions?

See `FIX_COORDINATE_BUG.md` for complete technical details, test results, and explanation of the fix.
