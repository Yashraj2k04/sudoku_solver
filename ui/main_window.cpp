#include "main_window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "sudoku.h"
#include <generator.h>
#include <solver.h>
#include <QIntValidator>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
}

void MainWindow::setupUI() {
    centralWidget = new QWidget(this);      //main widget
    setCentralWidget(centralWidget);

    setStyleSheet("background-color: #f0f0f0;");        //bg white

    backButton = new QPushButton("Back", this);
    backButton->setStyleSheet("background-color:transparent;"
                              "color:#000000;"
                              "font-size:15px;"
                              "border: none;"   
                              "padding: 5px;");     //making back button bland.
    QHBoxLayout *backButtonLayout = new QHBoxLayout();
    backButtonLayout->addWidget(backButton, 0, Qt::AlignLeft);  // back button to stick at top left of the screen (main widget)

    statusLabel = new QLabel("Difficulty: ", this);             //difficulty label to display difficulty
    statusLabel->setStyleSheet("font-size: 20px; font-weight: bold;");
    QHBoxLayout *difficultyLayout = new QHBoxLayout();
    difficultyLayout->addWidget(statusLabel, 0, Qt::AlignCenter);


    // grid layout 9x9.
    gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0);
    
    int cellSize = 54; // coz borders will be wider at every 3rd cell
    
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            QLineEdit *cell = new QLineEdit(this);
            cell->setFixedSize(cellSize, 50);   
            cell->setAlignment(Qt::AlignCenter);
            cell->setMaxLength(1);
            cell->setValidator(new QIntValidator(1, 9, this)); // makes it such that only 1-9 can be input (if the input is given from keyboard)
            cell->installEventFilter(this);
            cells[row][col] = cell;
    
            // Set custom borders per cell
            QString borderStyle;
            borderStyle += "QLineEdit {"
                           "background-color: #ffffff;"
                           "color: #000000;"
                           "font-size: 18px;"
                           "border-style: solid;"
                           "border-width: 1px;"
                           "border-color: #000000;"
                           "margin: 0px;"
                           "padding: 0px;";
    
            if (row % 3 == 0) borderStyle += "border-top-width: 4px;";      //normal styling for each, thicker borders for every 3rd cell
            if (col % 3 == 0) borderStyle += "border-left-width: 4px;";
            if (col == 8) borderStyle += "border-right-width: 4px;";
            if (row == 8) borderStyle += "border-bottom-width: 4px;";
    
            borderStyle += "}";     // addin enclosing braces
    
            cell->setStyleSheet(borderStyle);
    
            gridLayout->addWidget(cell, row, col);
    
            if (!cell->text().isEmpty()) {
                cell->setReadOnly(true);                // making pregenerated cells readable only
            } else {
                connect(cell, &QLineEdit::textChanged, this, &MainWindow::cellChanged);     // or connect the cell slot to signal this window when its text changes to call the cellChanged function
            }
        }
    }

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


    generateButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);      //expanding buttons
    solveButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout *buttonLayout = new QHBoxLayout();      
    buttonLayout->setSpacing(10);
    buttonLayout->addWidget(generateButton);
    buttonLayout->addWidget(solveButton);       //hbox for solve and generate buttons

    QWidget *buttonContainer = new QWidget(this);
    buttonContainer->setLayout(buttonLayout);
    buttonContainer->setFixedWidth(490);        // i approximated the width to be 490 ish, couldnt get both hbox and grid for 3x3 input to both have varying width to align with board width

    solveButton->setEnabled(false);

    inputGridLayout = new QGridLayout();
   
for (int i = 1; i <= 9; ++i) {
    QPushButton *btn = new QPushButton(QString::number(i), this);
    btn->setFixedSize(156, 60);  

    btn->setStyleSheet(
        "QPushButton {"
        "background-color:rgb(245, 129, 20);"
        "color: white;"
        "font-size: 18px;"
        "border-radius: 1px;"
        "padding: 14px;"
        "}"
        "QPushButton:hover {"
        "background-color: #1976D2;"
        "}"
    );

    connect(btn, &QPushButton::clicked, this, [this, i]() {
        if (selectedCell && !selectedCell->isReadOnly()) {
            selectedCell->setText(QString::number(i));
        }
    });     //changes qline value of selected cell of selected grid to be the button's number value when selected and clicked

    int row = (i - 1) / 3;
    int col = (i - 1) % 3;
    inputGridLayout->addWidget(btn, row, col);
}

    QWidget *inputContainer = new QWidget(this);
    QHBoxLayout *inputWrapper = new QHBoxLayout(inputContainer);
    inputWrapper->addStretch();
    inputWrapper->addLayout(inputGridLayout);
    inputWrapper->addStretch();
    inputContainer->setFixedWidth(490);  // same approximation as mentioned above in case of hbox

    QVBoxLayout *containerLayout = new QVBoxLayout();
    containerLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    containerLayout->addLayout(difficultyLayout);
    containerLayout->addLayout(gridLayout);
    containerLayout->addWidget(buttonContainer);
    containerLayout->addStretch();
    containerLayout->addWidget(inputContainer);     //adds every other layout to the container

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addLayout(backButtonLayout);
    mainLayout->addLayout(containerLayout);

    connect(backButton, &QPushButton::clicked, this, &MainWindow::backToDifficultyScreen);      //back to diff screen
    connect(generateButton, &QPushButton::clicked, this, [this]() { generateSudoku(currentHints); });   //generate board as according to the chosen difficulty
    connect(solveButton, &QPushButton::clicked, this, &MainWindow::solveSudoku);        // sovle button
}


bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress) {
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                if (obj == cells[row][col]) {

                    // Reset previous cell's border
                    QString prevStyle = cells[row][col]->styleSheet();
                    QString baseStyle = prevStyle;

                    baseStyle = baseStyle.replace(QRegularExpression("border: \\d+px solid blue;"), "");

                    // Reset all cells' styles (to be safe)
                    for (int r = 0; r < 9; ++r) {
                        for (int c = 0; c < 9; ++c) {
                            QString style = cells[r][c]->styleSheet();
                            style = style.replace(QRegularExpression("border: \\d+px solid blue;"), "");
                            cells[r][c]->setStyleSheet(style);
                        }
                    }

                    // Highlight current cell
                    QString selectedStyle = cells[row][col]->styleSheet();
                    selectedStyle += "border: 2px solid blue;";
                    cells[row][col]->setStyleSheet(selectedStyle);

                    // Most important: assign the cell so input grid knows what to update
                    selectedCell = cells[row][col];

                    return true;
                }
            }
        }
    }
    return QMainWindow::eventFilter(obj, event);
}

QString MainWindow::baseCellStyle(int row, int col) {
    QString style =
        "background-color: #ffffff;"
        "color: #000000;"
        "font-size: 18px;"
        "border-style: solid;"
        "border-width: 1px;"
        "border-color: black;"
        "margin: 0px;"
        "padding: 0px;";

    if (row % 3 == 0) style += "border-top-width: 4px;";
    if (col % 3 == 0) style += "border-left-width: 4px;";
    if (row == 8)     style += "border-bottom-width: 4px;";
    if (col == 8)     style += "border-right-width: 4px;";

    return style;
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
}

void MainWindow::solveSudoku() {
    

    Solver solver;
    solver.solveSudoku(board);

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            cells[row][col]->setText(QString::number(board[row][col]));
        }
    }

    generated = false;
    solveButton->setEnabled(false);
}

void MainWindow::cellChanged() {
    // Placeholder for validation or other logic
}
