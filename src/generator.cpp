#include "sudoku.h"
#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <random>

Sudoku::Sudoku() : board(9, vector<int>(9, 0)) {
    srand(time(0));
}

void Sudoku::generate() {
    board = vector<vector<int>>(9, vector<int>(9, 0)); // Reset board

    std::random_device rd;
    std::mt19937 g(rd());

    // Randomize board by filling diagonal 3x3 grids with unique numbers
    for (int i = 0; i < 9; i += 3) {
        vector<int> numbers(9);
        for (int j = 0; j < 9; ++j) numbers[j] = j + 1;
        std::shuffle(numbers.begin(), numbers.end(), g);

        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) {
                board[i + row][i + col] = numbers[row * 3 + col];
            }
        }
    }

    // Solve to get a complete valid board
    solveSudoku();

    // Remove numbers to create puzzle
    int clues = 30; // Number of pre-filled clues
    while (clues > 0) {
        int row = rand() % 9;
        int col = rand() % 9;
        if (board[row][col] != 0) {
            board[row][col] = 0;
            clues--;
        }
    }
}

void Sudoku::printBoard() {
    for (auto &row : board) {
        for (int num : row) {
            cout << num << " ";
        }
        cout << endl;
    }
}
