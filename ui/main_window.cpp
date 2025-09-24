#include "main_window.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include "sudoku.h"
#include <generator.h>
#include <solver.h>
#include <QIntValidator>
#include <QSet>
#include <QPair>
#include <QHash>
#include <QTimer>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setupUI();
    gameTimer = new QTimer(this);
    gameTimer->setInterval(1000); // update every second

    connect(gameTimer, &QTimer::timeout, this, [this]() {
        int elapsed = startTime.secsTo(QTime::currentTime());

        // Fix: handle case when currentTime is after midnight (wraparound)
        if (elapsed < 0)
            elapsed += 24 * 60 * 60; // add 86400 seconds (24 hours)

        int minutes = elapsed / 60;
        int seconds = elapsed % 60;

        timeLabel->setText(QString("%1:%2")
                            .arg(minutes, 2, 10, QLatin1Char('0'))
                            .arg(seconds, 2, 10, QLatin1Char('0')));
    });
}


void MainWindow::setupUI() {
    centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    setStyleSheet("background-color: #f0f0f0;");

    backButton = new QPushButton("Back", this);
    backButton->setStyleSheet("background-color:transparent;"
                              "color:#000000;"
                              "font-size:15px;"
                              "border: none;"
                              "padding: 5px;");
    QHBoxLayout *backButtonLayout = new QHBoxLayout();
    backButtonLayout->addWidget(backButton, 0, Qt::AlignLeft);  //back button at top left

    statusLabel = new QLabel("Difficulty: ", this);
    statusLabel->setStyleSheet("font-size: 20px; font-weight: bold;");
    QHBoxLayout *difficultyLayout = new QHBoxLayout();
    difficultyLayout->addWidget(statusLabel, 0, Qt::AlignCenter);
    QHBoxLayout *timeLayout = new QHBoxLayout();
    timeLabel = new QLabel("00:00",this);
    timeLayout->addWidget(timeLabel, 0, Qt::AlignCenter);


    //difficulty label at center

    gridLayout = new QGridLayout();
    gridLayout->setSpacing(0);
    gridLayout->setContentsMargins(0, 0, 0, 0); //9x9 grid for main board

    int cellSize = 54;

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            QLineEdit *cell = new QLineEdit(this);
            cell->setFixedSize(cellSize, 50);
            cell->setAlignment(Qt::AlignCenter);
            cell->setMaxLength(1);
            cell->setValidator(new QIntValidator(1, 9, this));      //lets only ints from 1 to 9 be entered in board
            cell->installEventFilter(this);
            cells[row][col] = cell;

            QString borderStyle;
            borderStyle += "QLineEdit {"
                           "background-color: #ffffff;"
                           "color: #000000;"
                           "font-size: 18px;"
                           "border-style: solid;"
                           "border-width: 1px;"
                           "border-color: #000000;"
                           "margin: 0px;"
                           "padding: 0px;"; //normal board style

            if (row % 3 == 0) borderStyle += "border-top-width: 4px;";
            if (col % 3 == 0) borderStyle += "border-left-width: 4px;";
            if (col == 8) borderStyle += "border-right-width: 4px;";
            if (row == 8) borderStyle += "border-bottom-width: 4px;";       //okay, kinda slick. we bolden every third row and column intersection to highlight 3x3 grids

            borderStyle += "}";

            cell->setStyleSheet(borderStyle);
            cell->setReadOnly(true);
            QRegExpValidator *emptyValidator = new QRegExpValidator(QRegExp(""), this);     //disabling keyboard input, only play from 3x3 i/p grid lil bro
            
            cells[row][col]->setValidator(emptyValidator);
            gridLayout->addWidget(cell, row, col);

            connect(cell, &QLineEdit::textChanged, this, &MainWindow::cellChanged);
            
            
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

    generateButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    solveButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);     //done so as to make them resize as per the 9x9 grid length
    

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    buttonLayout->addWidget(generateButton);
    buttonLayout->addWidget(solveButton);

    QWidget *buttonContainer = new QWidget(this);
    buttonContainer->setLayout(buttonLayout);
    buttonContainer->setFixedWidth(490);

    solveButton->setEnabled(false);     //set to false, disables user from pressing solve button if board is not generated yet

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
        );      //3x3 input grid for 1-9, i really need to change the color scheme here

        connect(btn, &QPushButton::clicked, this, [this, i]() {
            if (selectedCell && !selectedCell->isReadOnly()) {      // if selected cell exists and its not a read only (pregenerated)
                int row = -1, col = -1;                 //init rows and cols var
                for (int r = 0; r < 9; ++r) {
                    for (int c = 0; c < 9; ++c) {
                        if (cells[r][c] == selectedCell) {      
                            row = r;
                            col = c;
                            break;
                        }
                    }
                    if (row != -1) break;
                }

                if (row != -1 && col != -1) {
                    QString prev = selectedCell->text();
                    moveStack.push({row, col, prev});       //push prev value to move stack so we can revert to it sequentially
                    selectedCell->setText(QString::number(i));  //set number we had from input grid
                }
            }
        });

        int row = (i - 1) / 3;
        int col = (i - 1) % 3;
        inputGridLayout->addWidget(btn, row, col);
    }

    //revert Button
    QPushButton *revertButton = new QPushButton("Revert", this);
    revertButton->setFixedSize(150, 50);
    revertButton->setStyleSheet(
        "QPushButton {"
        "background-color: #e53935;"
        "color: white;"
        "font-size: 18px;"
        "border-radius: 10px;"
        "padding: 10px;"
        "}"
        "QPushButton:hover {"
        "background-color: #b71c1c;"
        "}"
    );

    connect(revertButton, &QPushButton::clicked, this, [this]() {
        if (!moveStack.empty()) {
            auto [row, col, prev] = moveStack.top();
            moveStack.pop();
            cells[row][col]->setText(prev);
        }
    });
    

    QHBoxLayout *revertLayout = new QHBoxLayout();
    revertLayout->addStretch();
    revertLayout->addWidget(revertButton);
    revertLayout->addStretch();

    QVBoxLayout *inputGridWithUndo = new QVBoxLayout();
    inputGridWithUndo->addLayout(inputGridLayout);
    inputGridWithUndo->addSpacing(10);
    inputGridWithUndo->addLayout(revertLayout);

    QWidget *inputContainer = new QWidget(this);
    QHBoxLayout *inputWrapper = new QHBoxLayout(inputContainer);
    inputWrapper->addStretch();
    inputWrapper->addLayout(inputGridWithUndo);
    inputWrapper->addStretch();
    inputContainer->setFixedWidth(490);

    QVBoxLayout *containerLayout = new QVBoxLayout();
    containerLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);
    containerLayout->addLayout(difficultyLayout);
    containerLayout->addLayout(timeLayout);
    containerLayout->addLayout(gridLayout);
    containerLayout->addWidget(buttonContainer);
    containerLayout->addStretch();
    containerLayout->addWidget(inputContainer);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->addLayout(backButtonLayout);
    mainLayout->addLayout(containerLayout);

    connect(backButton, &QPushButton::clicked, this, [this]() {
    // Reset all cells to blank, default style, and read-only
    for (int row = 0; row < 9; ++row) {
        gameTimer->stop();
        timeLabel->setText("00:00");

        for (int col = 0; col < 9; ++col) {
            QLineEdit *cell = cells[row][col];
            cell->clear();
            cell->setReadOnly(true);
            cell->setStyleSheet(
                "QLineEdit {"
                "background-color: #ffffff;"
                "color: #000000;"
                "font-size: 18px;"
                "border-style: solid;"
                "border-width: 1px;"
                "border-color: #000000;"
                "margin: 0px;"
                "padding: 0px;"
                + QString(row % 3 == 0 ? "border-top-width: 4px;" : "") +
                QString(col % 3 == 0 ? "border-left-width: 4px;" : "") +
                QString(col == 8 ? "border-right-width: 4px;" : "") +
                QString(row == 8 ? "border-bottom-width: 4px;" : "") +
                "}"
            );

            // Disable keyboard input
            QRegExpValidator *emptyValidator = new QRegExpValidator(QRegExp(""), this);
            cell->setValidator(emptyValidator);
        }
    }

    // Disable solve button
    solveButton->setEnabled(false);

    // Clear status label
    statusLabel->clear();
    MainWindow::backToDifficultyScreen();
});

    connect(generateButton, &QPushButton::clicked, this, [this]() { generateSudoku(currentHints); });
    connect(solveButton, &QPushButton::clicked, this, &MainWindow::solveSudoku);
    
}



bool MainWindow::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::MouseButtonPress && generated) {       //only selectable if board was generated.
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                if (obj == cells[row][col]) {       // if obj that sent the sent the signal MouseButtonPress was this col and row

                    if (cells[row][col]->isReadOnly())          //we only wanna highlight selectable cells (not pregenerated cells)
                        return true;

                    // Clear previous blue borders from ALL cells, the highlighted cell is reverted back to normal
                    for (int r = 0; r < 9; ++r) {
                        for (int c = 0; c < 9; ++c) {
                            QString style = cells[r][c]->styleSheet();
                            style = style.replace("border: 2px solid blue;", "");       
                            cells[r][c]->setStyleSheet(style);
                        }
                    }

                    // and make blue border for the selected cell
                    QString style = cells[row][col]->styleSheet();
                    if (!style.contains("border: 2px solid blue;")) {
                        style = style.replace("}", "border: 2px solid blue;}");
                        cells[row][col]->setStyleSheet(style);
                    }

 
                    selectedCell = cells[row][col];        // set the value that is to be passed
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
}       // as discussed above

void MainWindow::setHints(int hints) {
    currentHints = hints;
    QString difficultyText = "";
    if (hints == 35) difficultyText = "Difficulty: Easy";
    else if (hints == 20) difficultyText = "Difficulty: Medium";
    else if (hints==15) difficultyText = "Difficulty: Hard";

    statusLabel->setText(difficultyText);
}       // difficulty label set as per the selected difficulty

void MainWindow::generateSudoku(int hints) {

    startTime = QTime::currentTime();
    gameTimer->start();
    timeLabel->setText("00:00");  // reset display
    

    currentHints = hints;
    generated = true;       
    solveButton->setEnabled(true);      // generated is true, now enable solve button
    Generator generator;

    Difficulty difficulty;
    if (hints == 40) difficulty = Difficulty::EASY;
    else if (hints == 20) difficulty = Difficulty::MEDIUM;
    else difficulty = Difficulty::HARD;

    generator.generateSudoku(board, difficulty);

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (board[row][col] != 0) {
                cells[row][col]->setText(QString::number(board[row][col]));
                cells[row][col]->setReadOnly(true);
                cells[row][col]->setStyleSheet(baseCellStyle(row, col) + "background-color: #d3d3d3;"); 
            } else {
                cells[row][col]->clear();
                cells[row][col]->setReadOnly(false);
                cells[row][col]->setStyleSheet(baseCellStyle(row, col) + "background-color: #ffffff;");
            }
            // if a cell was pregenerated, make its background grey to make it distinguishable from other editable cells
            
        }
    }
    if(currentHints == 40){
        statusLabel->setText("Difficulty: Easy");
    }
    else if(currentHints == 20){
        statusLabel->setText("Difficulty: Medium");
    }
    else if(currentHints == 15){
        statusLabel->setText("Difficulty: Hard");
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
    solveButton->setEnabled(false);     //solves board. sets generated to false, disables enabled solve button


    cellChanged();
}

void MainWindow::cellChanged() {
    if (!generated) return;

    // Clear all current highlights
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            QString style = baseCellStyle(row, col);
            if (cells[row][col]->isReadOnly()) {
                style.replace("background-color: #ffffff;", "background-color: #d3d3d3;");
            }
            cells[row][col]->setStyleSheet("QLineEdit {" + style + "}");
        }
    }

    // Track conflicting cells
    QSet<QPair<int, int>> conflictCells;

    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            QString val = cells[row][col]->text();
            if (val.isEmpty()) continue;
            for (int c = 0; c < 9; ++c) {                       //check row
                if (c != col && cells[row][c]->text() == val) {
                    conflictCells.insert({row, col});
                    conflictCells.insert({row, c});
                }
            }
            for (int r = 0; r < 9; ++r) {                       //check col
                if (r != row && cells[r][col]->text() == val) {
                    conflictCells.insert({row, col});
                    conflictCells.insert({r, col});
                }
            }
            int startRow = (row / 3) * 3;
            int startCol = (col / 3) * 3;
            for (int r = startRow; r < startRow + 3; ++r) {     //check 3x3 sub-grid
                for (int c = startCol; c < startCol + 3; ++c) {
                    if ((r != row || c != col) && cells[r][c]->text() == val) {
                        conflictCells.insert({row, col});
                        conflictCells.insert({r, c});
                    }
                }
            }
        }
    } 
    for (const auto &cell : conflictCells) {
        int row = cell.first;
        int col = cell.second;  
        QString style = baseCellStyle(row, col);
        style.replace(QRegularExpression("background-color: #[0-9a-fA-F]{6};"), "background-color:rgb(253, 72, 72);");  // red highlight for conflicting cells which are present.
        cells[row][col]->setStyleSheet("QLineEdit {" + style + "}");
    }
    if (conflictCells.isEmpty() && isBoardCorrectlySolved()) {
        // All good and solved!
    }
}


bool MainWindow::isBoardCorrectlySolved() {
    // Check if any conflicting cells exist
    bool hasConflicts = false;
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (cells[row][col]->styleSheet() == "background-color:rgb(253, 72, 72);") {
                hasConflicts = true;
                break;
            }
        }
        if (hasConflicts) break;
    }

    // Check if all cells are filled (no empty cells)
    bool allFilled = true;
    for (int row = 0; row < 9; ++row) {
        for (int col = 0; col < 9; ++col) {
            if (cells[row][col]->text().isEmpty()) {
                allFilled = false;
                break;
            }
        }
        if (!allFilled) break;
    }



    // If no conflicts and all cells are filled, the board is correctly solved
    if (!hasConflicts && allFilled) {
        gameTimer->stop();
        statusLabel->setText("Solved!");
        while (!moveStack.empty()) moveStack.pop();
    
        int userId = -1;
    
        // Get user ID
        QSqlQuery queryUserId;
        queryUserId.prepare("SELECT id FROM users WHERE username = :username");
        queryUserId.bindValue(":username", currentUsername);
        if (!queryUserId.exec()) {
            qWarning() << "Failed to get user_id:" << queryUserId.lastError().text();
            return false;
        }
        if (queryUserId.next()) {
            userId = queryUserId.value(0).toInt();
        } else {
            qWarning() << "User not found in database!";
            return false;
        }
    
        // Determine next game number
        int gameNumber = 1;
        QSqlQuery queryMaxGame;
        queryMaxGame.prepare("SELECT MAX(game_number) FROM game_sessions WHERE user_id = :user_id");
        queryMaxGame.bindValue(":user_id", userId);
        if (queryMaxGame.exec() && queryMaxGame.next() && !queryMaxGame.value(0).isNull()) {
            gameNumber = queryMaxGame.value(0).toInt() + 1;
        }
    
        // Insert game session
        int durationSeconds = startTime.secsTo(QTime::currentTime());
        QSqlQuery queryInsert;
        queryInsert.prepare("INSERT INTO game_sessions (user_id, game_number, duration_seconds) "
                            "VALUES (:user_id, :game_number, :duration)");
        queryInsert.bindValue(":user_id", userId);
        queryInsert.bindValue(":game_number", gameNumber);
        queryInsert.bindValue(":duration", durationSeconds);
    
        if (!queryInsert.exec()) {
            qDebug() << "Insert failed:" << queryInsert.lastError().text();
        } else {
            qDebug() << "Game session inserted.";
        }
    
        return true;
    }
    


    return false;
}





void MainWindow::setUsername(const QString &username) {
    this->currentUsername = username;  // assuming you have a member QString currentUsername;
}
