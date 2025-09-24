#include "generator.h"
#include <random>
#include <algorithm>

Generator::Generator() {
    board.fill({0});  // start a board with zeros for 9x9
}

void Generator::generateSudoku(std::array<std::array<int, 9>, 9>& outBoard, Difficulty difficulty) {
    fillBoard();        // fill up the board to make a valid sudoku solution

    int clues;
    switch (difficulty) {
        case Difficulty::EASY: clues = 40; break;
        case Difficulty::MEDIUM: clues = 20; break;
        case Difficulty::HARD: clues = 15; break;
    }
                        // how many clues we leave depending upon the difficulty

    removeNumbers(81 - clues);
    outBoard = board;   
}

bool Generator::fillBoard() {       // fill up the board
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (board[row][col] == 0) {
                std::vector<int> numbers(9);
                std::iota(numbers.begin(), numbers.end(), 1);   // place numbers from 1 to 9, (increasing order) . similar to writing numbers = {1,2,3,4,5,6,7,8,9},
                std::shuffle(numbers.begin(), numbers.end(), std::mt19937(std::random_device()())); //shuffle using random device seed
                
                for (int num : numbers) {
                    if (isSafe(row, col, num)) {    //if is safe to place the random number 
                        board[row][col] = num;      //then we place it, just ensuring we make a random board thats valid.
                        if (fillBoard()) return true;
                        board[row][col] = 0;
                    }
                }
                return false;
            }
        }
    }
    return true;
}

bool Generator::isSafe(int row, int col, int num) {     // basic validator for sudoku, if it works for the current cell to be placed
    for (int i = 0; i < 9; ++i) {
        if (board[row][i] == num || board[i][col] == num) return false;
    }

    int startRow = (row / 3) * 3, startCol = (col / 3) * 3;
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[startRow + i][startCol + j] == num) return false;
        }
    }

    return true;
}

void Generator::removeNumbers(int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 8);
            //randomizing the cells that will be actually prefilled

    while (count > 0) {
        int row = dis(gen);
        int col = dis(gen);
        if (board[row][col] != 0) {
            board[row][col] = 0;
            --count;
        }
    }
}
