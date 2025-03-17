#include "sudoku.h"
#include <algorithm>
#include <random>

bool Sudoku::isValid(int row, int col, int num) {
    for (int i = 0; i < 9; ++i) {
        if (board[row][i] == num || board[i][col] == num || board[row - row % 3 + i / 3][col - col % 3 + i % 3] == num) {
            return false;
        }
    }
    return true;
}

bool Sudoku::solveSudoku() {
    std::random_device rd;
    std::mt19937 g(rd());

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (board[row][col] == 0) {
                vector<int> numbers(9);
                for (int i = 0; i < 9; ++i) numbers[i] = i + 1;
                std::shuffle(numbers.begin(), numbers.end(), g);

                for (int num : numbers) {
                    if (isValid(row, col, num)) {
                        board[row][col] = num;
                        if (solveSudoku()) return true;
                        board[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

bool Sudoku::solve() {
    return solveSudoku();
}

bool Sudoku::validate() {
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            int num = board[row][col];
            if (num != 0) {
                board[row][col] = 0; // Temporarily empty the cell
                if (!isValid(row, col, num)) {
                    board[row][col] = num; // Restore the value
                    return false;
                }
                board[row][col] = num; // Restore the value
            }
        }
    }
    return true;
}
