#include "sudoku.h"
#include <iostream>
using namespace std;

int main() {
    Sudoku sudoku;
    sudoku.generate();
    cout << "Generated Sudoku Puzzle:\n";
    sudoku.printBoard();
    return 0;
}