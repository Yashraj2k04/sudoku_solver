#include "sudoku.h"
#include <iostream>
using namespace std;

int main() {
    Sudoku sudoku;
    sudoku.generate();
    cout << "Generated Sudoku Puzzle:\n";
    sudoku.printBoard();

    if (sudoku.solve()) {
        cout << "\nSolved Sudoku Puzzle:\n";
        sudoku.printBoard();
    } else {
        cout << "\nNo solution found." << endl;
    }

    if (sudoku.validate()) {
        cout << "\nSudoku board is valid." << endl;
    } else {
        cout << "\nSudoku board is invalid." << endl;
    }

    return 0;
}