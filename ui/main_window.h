// i nearly died tryna write the input logic from 3x3

#ifndef MAIN_WINDOW_H       // include guard, like pragma once
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QEvent>
#include <array>
#include <stack>
#include <tuple>
#include <QTimer>
#include <QTime>
class MainWindow : public QMainWindow {
    Q_OBJECT                                            //Object that communicates with other windows

public:
    MainWindow(QWidget *parent = nullptr);              // sets difficulty level
    void setHints(int hints);

    void setUsername(const QString &username);

signals:
    void backToDifficultyScreen();                      // signal to take us back to select difficulty

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;     //method of QOBJECT. did so that we can intercept event click or changed before it reaches the obj
                                                                // here we do it to store the selected cell.
                                                                // if i didnt do that, it was changing the last editable cell insted of the selected cell
                                                                // this makes it wait for the qline to be edited before it process ui change
                                                                QString baseCellStyle(int row, int col);
private slots:
    void generateSudoku(int hints);             // slots that sends signals to perform some action
    void solveSudoku();
    void cellChanged();

private:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QPushButton *solveButton;
    QPushButton *generateButton;
    QPushButton *backButton;
    QLineEdit *cells[9][9];
    QLabel *statusLabel;
    QLabel *timeLabel;
    QTimer *gameTimer;
    QTime startTime;


    QString currentUsername;

    QGridLayout *inputGridLayout;              // the 3x3 input grid 
    QLineEdit *selectedCell = nullptr;          // cell that we selected to edit

    void setupUI();
    void updateBoardFromUI();
    void updateUIFromBoard();

    int currentHints;
    std::array<std::array<int, 9>, 9> board;
    bool generated = false;
    std::stack<std::tuple<int, int, QString>> moveStack;


    bool isBoardCorrectlySolved();          // method to check if board is solved.
};

#endif
