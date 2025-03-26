// making a new header file. now i am completely eliminating the code that will be working on the logic for terminal based sudoku
// sudoku is now switching from terminal gaming to ui gaming babyyy!!


#ifndef GENERATOR_H             //include guard
#define GENERATOR_H

#include <array>

// enum to represent difficulty levels
enum class Difficulty {
    EASY,
    MEDIUM,
    HARD
};      //these will be the basic difficulty levels for now. will determine how many ccells are pre-filled.

class Generator {
public:
    Generator();  // generic constructor, start up a basic board 9x9.
    void generateSudoku(std::array<std::array<int, 9>, 9>& outBoard, Difficulty difficulty);  // generate a sudoku for given difficulty

private:
    std::array<std::array<int, 9>, 9> board;  // 9x9 Sudoku board

    bool fillBoard();  // fill the board with a solution
    bool isSafe(int row, int col, int num);  // check if placing 'num' at (row, col) is valid
    void removeNumbers(int count);  // remove numbers to create the puzzle with given clues
};

#endif      