#include "sudoku.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

Sudoku::Sudoku() : board(9, vector<int>(9, 0)) {
    srand(time(0));
}

void Sudoku::generate() {
    // Fill the board with a valid solution
    solveSudoku(); 

    // Remove numbers to create a puzzle
    int cluesToRemove = 30;  // Keep 30 clues
    while (cluesToRemove > 0) {
        int row = rand() % 9;
        int col = rand() % 9;
        if (board[row][col] != 0) {
            board[row][col] = 0;
            cluesToRemove--;
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