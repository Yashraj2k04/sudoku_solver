#include "solver.h"

Solver::Solver() {}     //nothing to do with constructor obv.

bool Solver::solveSudoku(std::array<std::array<int, 9>, 9>& board) {
    int row, col;
    if (!findEmptyCell(board, row, col)) {
        return true;  // No empty cells left, puzzle is solved
    }

    for (int num = 1; num <= 9; ++num) {
        if (isValidMove(board, row, col, num)) {        //same logic, can find the comments of generator.cpp, not gonna explain allat.
            board[row][col] = num;
            if (solveSudoku(board)) {
                return true;
            }
            board[row][col] = 0; 
        }
    }

    return false;
}

bool Solver::isValidMove(const std::array<std::array<int, 9>, 9>& board, int row, int col, int num) {
    for (int i = 0; i < 9; ++i) {
        if (board[row][i] == num || board[i][col] == num) {
            return false;
        }
    }
    int startRow = row - row % 3;
    int startCol = col - col % 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[startRow + i][startCol + j] == num) {
                return false;
            }
        }
    }
    return true;
}

bool Solver::findEmptyCell(const std::array<std::array<int, 9>, 9>& board, int& row, int& col) {
    for (row = 0; row < 9; ++row) {
        for (col = 0; col < 9; ++col) {
            if (board[row][col] == 0) {
                return true;
            }
        }
    }
    return false;
}
