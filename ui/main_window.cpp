#include "main_window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "sudoku.h"
#include <generator.h>
#include <solver.h>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
}

void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    setStyleSheet("background-color: #f0f0f0;");

    // 🎮 Back Button (Floating Top-Left)
    backButton = new QPushButton("Back", this);
    backButton->setStyleSheet("background-color:transparent;"
                              "color:#000000;"
                              "font-size:15px;"
                              "border: none;"
                              "padding: 5px;");

    QHBoxLayout *backButtonLayout = new QHBoxLayout();
    backButtonLayout->addWidget(backButton, 0, Qt::AlignLeft);

    // 🌟 Difficulty Label HBox
    statusLabel = new QLabel("Difficulty: ", this);
    statusLabel->setStyleSheet("font-size: 20px; font-weight: bold;");
    
    QHBoxLayout *difficultyLayout = new QHBoxLayout();
    difficultyLayout->addWidget(statusLabel, 0, Qt::AlignCenter);

    // 🎮 Create Grid Layout
    gridLayout = new QGridLayout();
    gridLayout->setSpacing(5);

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            cells[row][col] = new QLineEdit(this);
            cells[row][col]->setFixedSize(50, 50);
            cells[row][col]->setAlignment(Qt::AlignCenter);
            cells[row][col]->setMaxLength(1);
            cells[row][col]->setStyleSheet(
                "border: 1px solid #888;"
                "font-size: 18px;"
                "background-color: #ffffff;"
                "color: #000000;"
            );
            gridLayout->addWidget(cells[row][col], row, col);

            if (!cells[row][col]->text().isEmpty()) {
                cells[row][col]->setReadOnly(true);
            } else {
                connect(cells[row][col], &QLineEdit::textChanged, this, &MainWindow::cellChanged);
            }
        }
    }

    // 🎮 Generate & Solve Buttons Styling
    generateButton = new QPushButton("Generate", this);
    solveButton = new QPushButton("Solve", this);
    
    QString buttonStyle =
        "QPushButton {"
        "background-color: #4CAF50;"
        "color: white;"
        "font-size: 18px;"
        "border-radius: 10px;"
        "padding: 10px;"
        "min-width: 120px;"
        "}"
        "QPushButton:hover {"
        "background-color: #388E3C;"
        "}";

    generateButton->setStyleSheet(buttonStyle);
    solveButton->setStyleSheet(buttonStyle);

    // 🌟 Buttons HBox
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(generateButton);
    buttonLayout->addWidget(solveButton);

    solveButton->setEnabled(false);

    // 🟢 Container for Difficulty, Grid, and Buttons
    QVBoxLayout *containerLayout = new QVBoxLayout();
    containerLayout->addLayout(difficultyLayout); // Difficulty Label
    containerLayout->addLayout(gridLayout);       // Sudoku Grid
    containerLayout->addLayout(buttonLayout);     // Buttons

    containerLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    // 🌟 Main Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addLayout(backButtonLayout); // Back Button Floating
    mainLayout->addLayout(containerLayout);  // Centered Container

    setLayout(mainLayout);

    // 🎮 Connections
    connect(backButton, &QPushButton::clicked, this, &MainWindow::backToDifficultyScreen);
    connect(generateButton, &QPushButton::clicked, this, [this]() { generateSudoku(currentHints); });
    connect(solveButton, &QPushButton::clicked, this, &MainWindow::solveSudoku);
}

void MainWindow::setHints(int hints) {
    currentHints = hints;
    QString difficultyText;
    if (hints == 30) difficultyText = "Difficulty: Easy";
    else if (hints == 20) difficultyText = "Difficulty: Medium";
    else difficultyText = "Difficulty: Hard";

    statusLabel->setText(difficultyText);
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

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (board[row][col] != 0) {
                cells[row][col]->setText(QString::number(board[row][col]));
                cells[row][col]->setReadOnly(true);
            } else {
                cells[row][col]->clear();
                cells[row][col]->setReadOnly(false);
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
    solver.solveSudoku(board);

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            cells[row][col]->setText(QString::number(board[row][col]));
        }
    }

    statusLabel->setText("Sudoku Solved!");

    generated = false;
    solveButton->setEnabled(false);
}

void MainWindow::cellChanged() {
    statusLabel->setText("Board Validated!");
}
