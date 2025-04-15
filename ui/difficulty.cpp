#include "difficulty.h"
#include <QFont>
#include <QHBoxLayout>
#include <QVBoxLayout>

Difficulty::Difficulty(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
}

void Difficulty::setupUI() {
    setStyleSheet("background-color: #f0f0f0;");

    QFont buttonFont;
    buttonFont.setBold(true);

    easyButton = new QPushButton("Easy", this);
    mediumButton = new QPushButton("Medium", this);
    hardButton = new QPushButton("Hard", this);
    backButton = new QPushButton("Back", this);

    QString buttonStyle =
        "QPushButton {"
        "background-color:rgb(124, 255, 128);"
        "color: white;"
        "font-size: 18px;"
        "border-radius: 15px;"
        "padding: 10px;"
        "min-width: 400px;" 
        "min-height: 50px;"
        "max-width: 600px;"  
        "max-height: 50px;"
        "margin-bottom: 10px;" 
        "}"
        "QPushButton:hover {"
        "background-color:rgb(148, 255, 153);"
        "}";

    easyButton->setStyleSheet(buttonStyle);
    mediumButton->setStyleSheet(buttonStyle);
    hardButton->setStyleSheet(buttonStyle);
    backButton->setStyleSheet("background-color:transparent;"
        "color:#000000;"
        "font-size:15px;"
        "border: none;"
        "padding: 5px;"); // did it same as the back button in login page


    easyButton->setFont(buttonFont);
    mediumButton->setFont(buttonFont);
    hardButton->setFont(buttonFont);

    //vbox to containerize the difficulty selection buttons
    QVBoxLayout *vbox = new QVBoxLayout();
    vbox->setAlignment(Qt::AlignCenter);
    vbox->addWidget(easyButton);
    vbox->addWidget(mediumButton);
    vbox->addWidget(hardButton);

    //hbox to set back button at the top left
    QHBoxLayout *hbox = new QHBoxLayout();
    hbox->addWidget(backButton, 0, Qt::AlignLeft);

    //main layout. we add hbox first to set back button at top left, and vbox for buttons to be in middle
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(hbox);
    mainLayout->addLayout(vbox);
    setLayout(mainLayout);
}

void Difficulty::setupConnections() {
    connect(easyButton, &QPushButton::clicked, this, [this]() {
        emit easySelected(35);
        emit difficultySelected(35);
    });
    connect(mediumButton, &QPushButton::clicked, this, [this]() {
        emit mediumSelected(20);
        emit difficultySelected(20);
    });
    connect(hardButton, &QPushButton::clicked, this, [this]() {
        emit hardSelected(15);
        emit difficultySelected(15);
    });
    connect(backButton, &QPushButton::clicked, this, &Difficulty::backToMainMenu);
}
