#ifndef MAIN_WINDOW_H                               //if not defined the main_window_h, 
#define MAIN_WINDOW_H                               // then only define main_window_h, skip to #endif otherwise

#include <QMainWindow>                  //Main window 
#include <QGridLayout>                  // grid layout for sudoku grid 9x9
#include <QLineEdit>                    // input fields for each cell
#include <QPushButton>                  // pushable buttons for solve button, generate button etc
#include <QLabel>                       // just a label, can write sudoku # or timer or something

#include<array>

class MainWindow : public QMainWindow{          // MainWindow inherits QMainWindow
    Q_OBJECT                                                        // ESSENTIAL !!! this is how Qt widgets (objects??) communicate with eachother.

    public:
        MainWindow(QWidget *parent = nullptr);  //generic constructor , it has no parent

        void setHints(int hints);  // to set total hints
    
    private slots:                              //private slots declare functions that'll be connected to signals
        void generateSudoku(int hints);                  //to generate sudoku
        void solveSudoku();                     // to solve the board for da noobs
        void cellChanged();                     //validate the board when user inputs in a cell
    
    private:
        QWidget *centralWidget;                 //pointers to widgets  here, nothing out of the ordinary
        QGridLayout *gridLayout;                
        QPushButton *solveButton;
        QPushButton *generateButton;
        QLineEdit *cells[9][9];
        QLabel *statusLabel;

        void setupUI();                         //setup the ui / load the window components
        void updateBoardFromUI();               
        void updateUIFromBoard();

        int currentHints;




        std::array<std::array<int, 9>, 9> board;
        bool generated = false;
};

#endif