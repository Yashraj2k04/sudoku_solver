#include "difficulty.h"
#include <QFont>                
#include <QGridLayout>

Difficulty::Difficulty(QWidget *parent) : QWidget(parent) {
    setupUI();
    setupConnections();
}

void Difficulty::setupUI() {
    QFont buttonFont;
    buttonFont.setBold(true);

    easyButton = new QPushButton("Easy", this);
    mediumButton = new QPushButton("Medium", this);
    hardButton = new QPushButton("Hard", this);
    backButton = new QPushButton("Back", this);

    easyButton->setFont(buttonFont);
    mediumButton->setFont(buttonFont);
    hardButton->setFont(buttonFont);
    backButton->setFont(buttonFont);

    QGridLayout *layout = new QGridLayout(this);
    layout->addWidget(backButton, 0, 0, Qt::AlignLeft);
    layout->addWidget(easyButton, 1, 1, Qt::AlignCenter);       //adding buttons to grid layout
    layout->addWidget(mediumButton, 2, 1, Qt::AlignCenter);
    layout->addWidget(hardButton, 3, 1, Qt::AlignCenter);

    setLayout(layout);
}

void Difficulty::setupConnections() {
    connect(easyButton, &QPushButton::clicked, this, [this]() {
        emit easySelected(30);
        emit difficultySelected(30);
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


            //kinda dank. lets see whats bussin here.

            //connect is a fn that connects a signal to a slot.
            //here, say medium button.
            // it emits a signal 'clicked'
            // 'this' refers to the current instance of the difficulty class
            //[this]() { emit mediumSelected(20) we call this a lambda function (kind like arrow fn in js???), it captures the surrounding scope '[this]'
            // here, the lambda emits that mediumSelected signal with a parameter of 20
}
