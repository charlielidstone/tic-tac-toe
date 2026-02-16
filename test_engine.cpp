//#include <iostream>
//#include <cassert>
//#include <string>
//#include "engine.hpp"
//#include "board.hpp"
//
///**
// * @brief Test suite for Engine minimax algorithm
// */
//class EngineTests {
//private:
//    int passedTests = 0;
//    int failedTests = 0;
//
//    // Helper function to print test results
//    void printTestResult(const std::string& testName, bool passed) {
//        std::cout << (passed ? "[PASS] " : "[FAIL] ") << testName << std::endl;
//        if (passed) {
//            passedTests++;
//        }
//        else {
//            failedTests++;
//        }
//    }
//
//    // Test 1: Minimax should return +10 (minus depth) for an immediate O win
//    void test_MinimaxImmediateOWin() {
//        Engine engine;
//        Board board;
//
//        // Setup: O has winning position on top row
//        board.setCell(0, 0, 'O');
//        board.setCell(0, 1, 'O');
//        board.setCell(0, 2, 'O');
//
//        int score = engine.minimax(board, true, 0);
//        bool passed = (score == 10); // Depth 0, so 10 - 0 = 10
//        printTestResult("Minimax Immediate O Win", passed);
//        assert(passed);
//    }
//
//    // Test 2: Minimax should return -10 (plus depth) for an immediate X win
//    void test_MinimaxImmediateXWin() {
//        Engine engine;
//        Board board;
//
//        // Setup: X has winning position on top row
//        board.setCell(0, 0, 'X');
//        board.setCell(0, 1, 'X');
//        board.setCell(0, 2, 'X');
//
//        int score = engine.minimax(board, true, 0);
//        bool passed = (score == -10); // Depth 0, so -10 + 0 = -10
//        printTestResult("Minimax Immediate X Win", passed);
//        assert(passed);
//    }
//
//    // Test 3: Minimax should return 0 for a draw
//    void test_MinimaxDraw() {
//        Engine engine;
//        Board board;
//
//        // Setup: Draw position
//        board.setCell(0, 0, 'X');
//        board.setCell(0, 1, 'O');
//        board.setCell(0, 2, 'X');
//        board.setCell(1, 0, 'X');
//        board.setCell(1, 1, 'X');
//        board.setCell(1, 2, 'O');
//        board.setCell(2, 0, 'O');
//        board.setCell(2, 1, 'X');
//        board.setCell(2, 2, 'O');
//
//        int score = engine.minimax(board, true, 0);
//        bool passed = (score == 0);
//        printTestResult("Minimax Draw", passed);
//        assert(passed);
//    }
//
//    // Test 4: O should block X from winning
//    void test_FindBestMove_BlockXWin() {
//        Engine engine;
//        Board board;
//
//        // Setup: X has two in a row, O should block
//        board.setCell(0, 0, 'X');
//        board.setCell(0, 1, 'X');
//        // Position (0, 2) should be the best move for O
//
//        auto bestMove = engine.findBestMove(board);
//        bool passed = (bestMove.first == 0 && bestMove.second == 2);
//        printTestResult("Find Best Move - Block X Win", passed);
//        assert(passed);
//    }
//
//    // Test 5: O should take the winning move
//    void test_FindBestMove_TakeWin() {
//        Engine engine;
//        Board board;
//
//        // Setup: O has two in a row and should take the win
//        board.setCell(0, 0, 'O');
//        board.setCell(0, 1, 'O');
//        board.setCell(1, 0, 'X');
//        board.setCell(2, 0, 'X');
//
//        auto bestMove = engine.findBestMove(board);
//        bool passed = (bestMove.first == 0 && bestMove.second == 2);
//        printTestResult("Find Best Move - Take Win", passed);
//        assert(passed);
//    }
//
//    // Test 6: O should take center on empty board
//    void test_FindBestMove_EmptyBoard() {
//        Engine engine;
//        Board board;
//
//        auto bestMove = engine.findBestMove(board);
//        // On an empty board, center (1,1) or corner are optimal
//        bool passed = ((bestMove.first == 1 && bestMove.second == 1) ||
//            (bestMove.first == 0 && bestMove.second == 0) ||
//            (bestMove.first == 0 && bestMove.second == 2) ||
//            (bestMove.first == 2 && bestMove.second == 0) ||
//            (bestMove.first == 2 && bestMove.second == 2));
//        printTestResult("Find Best Move - Empty Board", passed);
//        assert(passed);
//    }
//
//    // Test 7: Minimax prefers faster wins (lower depth)
//    void test_MinimaxPrefersFasterWin() {
//        Engine engine;
//        Board board;
//
//        // Setup: O can win in 1 move
//        board.setCell(0, 0, 'O');
//        board.setCell(0, 1, 'O');
//        // O should choose (0,2) to win immediately
//
//        auto bestMove = engine.findBestMove(board);
//        bool passed = (bestMove.first == 0 && bestMove.second == 2);
//        printTestResult("Minimax Prefers Faster Win", passed);
//        assert(passed);
//    }
//
//    // Test 8: O should block diagonal win
//    void test_FindBestMove_BlockDiagonal() {
//        Engine engine;
//        Board board;
//
//        // Setup: X threatens diagonal win
//        board.setCell(0, 0, 'X');
//        board.setCell(1, 1, 'X');
//        // O should block at (2, 2)
//
//        auto bestMove = engine.findBestMove(board);
//        bool passed = (bestMove.first == 2 && bestMove.second == 2);
//        printTestResult("Find Best Move - Block Diagonal", passed);
//        assert(passed);
//    }
//
//    // Test 9: O should prioritize winning over blocking
//    void test_FindBestMove_WinOverBlock() {
//        Engine engine;
//        Board board;
//
//        // Setup: O can win, X also threatens
//        board.setCell(0, 0, 'O');
//        board.setCell(0, 1, 'O');
//        board.setCell(1, 0, 'X');
//        board.setCell(1, 1, 'X');
//        // O should take win at (0,2) instead of blocking at (1,2)
//
//        auto bestMove = engine.findBestMove(board);
//        bool passed = (bestMove.first == 0 && bestMove.second == 2);
//        printTestResult("Find Best Move - Win Over Block", passed);
//        assert(passed);
//    }
//
//    // Test 10: Test column win detection
//    void test_FindBestMove_TakeColumnWin() {
//        Engine engine;
//        Board board;
//
//        // Setup: O has two in middle column
//        board.setCell(0, 1, 'O');
//        board.setCell(1, 1, 'O');
//        board.setCell(0, 0, 'X');
//
//        auto bestMove = engine.findBestMove(board);
//        bool passed = (bestMove.first == 2 && bestMove.second == 1);
//        printTestResult("Find Best Move - Take Column Win", passed);
//        assert(passed);
//    }
//
//public:
//    /**
//     * @brief Run all engine tests
//     * @return 0 if all tests pass, 1 if any test fails
//     */
//    int runAll() {
//        std::cout << "Running Engine Minimax Tests...\n" << std::endl;
//
//        passedTests = 0;
//        failedTests = 0;
//
//        try {
//            test_MinimaxImmediateOWin();
//            test_MinimaxImmediateXWin();
//            test_MinimaxDraw();
//            test_FindBestMove_BlockXWin();
//            test_FindBestMove_TakeWin();
//            test_FindBestMove_EmptyBoard();
//            test_MinimaxPrefersFasterWin();
//            test_FindBestMove_BlockDiagonal();
//            test_FindBestMove_WinOverBlock();
//            test_FindBestMove_TakeColumnWin();
//
//            std::cout << "\n======================================" << std::endl;
//            std::cout << "Tests Passed: " << passedTests << std::endl;
//            std::cout << "Tests Failed: " << failedTests << std::endl;
//            std::cout << "======================================" << std::endl;
//
//            if (failedTests == 0) {
//                std::cout << "✓ All tests passed!" << std::endl;
//                return 0;
//            }
//            else {
//                std::cout << "✗ Some tests failed!" << std::endl;
//                return 1;
//            }
//        }
//        catch (const std::exception& e) {
//            std::cerr << "\n✗ Test failed with exception: " << e.what() << std::endl;
//            return 1;
//        }
//    }
//};
//
//// Standalone function to run from main
//int runEngineTests() {
//    EngineTests tests;
//    return tests.runAll();
//}