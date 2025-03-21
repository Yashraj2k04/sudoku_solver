#ifndef SOLVER_H
#define SOLVER_H

#include <array>

class Solver {
public:
    Solver();  // constructor 

    bool solveSudoku(std::array<std::array<int, 9>, 9>& board); 

private:

    bool isValidMove(const std::array<std::array<int, 9>, 9>& board, int row, int col, int num);
    bool findEmptyCell(const std::array<std::array<int, 9>, 9>& board, int& row, int& col);
    
};

#endif
