#ifndef SUDOKU_H
#define SUDOKU_H

#include <vector>
using namespace std;

class Sudoku {
private:
    vector<vector<int>> board;
    bool isValid(int row, int col, int num);
    bool solveSudoku();

public:
    Sudoku();
    void generate();
    void printBoard();
    bool solve();
    bool validate();
};

#endif