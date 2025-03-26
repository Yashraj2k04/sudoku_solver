#include "main_window.h"
#include <QVBoxLayout>
#include "sudoku.h"
#include <QMessageBox>
#include <generator.h>
#include <solver.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent){      // when MainWindow is created, its constructor is called
    setupUI();                                                      // constructor call for the ui to set itself up
}

void MainWindow::setupUI(){

    centralWidget = new QWidget(this);                      //central widget in the window. the 'this' references to the ccurrent (MainWindow) it means Mainwindow will own the central widget
    setCentralWidget(centralWidget);
                                                        //makes central widget as the central widget lol, it means that central widget should act as main container, and destroy itself when main window closes 
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget); // Q vertical box layout, stacks widgets from top to bot. mainlayout is the layout manager for central widget

    gridLayout = new QGridLayout(centralWidget);        //same story. main layout is responsible for the grid layout inside it
    mainLayout->addLayout(gridLayout);

    for(int row = 0 ; row < 9; ++row){
        for(int col = 0 ; col < 9 ; ++col){
            cells[row][col] = new QLineEdit(this);
            cells[row][col]->setFixedSize(40,40);
            cells[row][col]->setAlignment(Qt::AlignCenter); // for each label that we call cells, (total 81), we initialize them and add to the grid layout
            cells[row][col]->setMaxLength(1);
            gridLayout->addWidget(cells[row][col],row,col);

            if(!cells[row][col]->text().isEmpty()){     //if a cell is pregenerated, it shouldn't be changable by the user, so we make it read only
                cells[row][col]->setReadOnly(true);
            }
            else{
                connect(cells[row][col], &QLineEdit::textChanged, this, &MainWindow::cellChanged); 
            }

            // connect is a signal-slot  connection function. here, the cell is the sender of the signal 'textChanged'. 'this' means this window, here, the MainWindow object will be the reciever
            // after 'this' recieves a signal 'txt changed' by the cell, it calls the cellChanged function inside mainWindow.
            // sheesh, too much going on here, but i got it down now.
        }
    }


    generateButton = new QPushButton("Generate", this); // initialized all the button pointers we made in header file, it reads the text inside as parameter
    solveButton = new QPushButton("Solve",this);        // and 'this' sets the parent of the buttons to be the MainWindow in this case
    statusLabel = new QLabel("Status: Ready", this);


    mainLayout->addWidget(generateButton);  // add these widges to mainlayout, thats it.
    mainLayout->addWidget(solveButton);
    mainLayout->addWidget(statusLabel);


    connect(generateButton, &QPushButton::clicked, this, [this]() { generateSudoku(currentHints); }); //added lambda
    connect(solveButton, &QPushButton::clicked, this, &MainWindow::solveSudoku);
    // same story, signal sent from  button saying it is clicked, this window recieves it and calls its respective function.

    solveButton->setEnabled(false);

}

void MainWindow::setHints(int hints) {
    currentHints = hints;
}

void MainWindow::generateSudoku(int hints) {
    currentHints = hints;
    generated = true;
    solveButton->setEnabled(true);
    Generator generator;


    Difficulty difficulty;
    if (hints == 30) difficulty = Difficulty::EASY;
    else if (hints == 20) difficulty = Difficulty::MEDIUM;
    else difficulty = Difficulty::HARD;

    generator.generateSudoku(board, difficulty);

    // updating the ui
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (board[row][col] != 0) {
                cells[row][col]->setText(QString::number(board[row][col]));
                cells[row][col]->setReadOnly(true);  // dont want user to change the prefilled, puzzle board
            } else {
                cells[row][col]->clear();
                cells[row][col]->setReadOnly(false); // else let the user change the cells.
            }
        }
    }

    statusLabel->setText("Sudoku Generated!");
}

void MainWindow::solveSudoku() {

    if (!generated) {
        statusLabel->setText("Generate a board first!");
        return;
    }
    
    Solver solver;
    solver.solveSudoku(board);              // we pass the generated board to auto solve. (we will have to change the logic by a huge extent once we make 
                                            // cellChanged function work next) :skull:
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            cells[row][col]->setText(QString::number(board[row][col]));
        }
    }

    statusLabel->setText("Sudoku Solved!");

    generated = false;
    solveButton->setEnabled(false);
}

void MainWindow::cellChanged(){
    statusLabel->setText("Board Validated!");
}
