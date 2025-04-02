// the game screen that is shown to the pplayer once they start the game


#include "main_menu.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

MainMenu::MainMenu(QWidget *parent) : QWidget(parent) {
    setupUI();                  //setup UI for the title screen
}

void MainMenu::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);        //vertical box layout

    titleLabel = new QLabel("SUDOKU CURVE", this);
    titleLabel->setAlignment(Qt::AlignCenter);              //self explanatory
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(24);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);


    loginButton = new QPushButton("Login", this);
    mainLayout->addWidget(loginButton);                                             // initialize qbutton, add to main layout (the vertical box one)
    connect(loginButton, &QPushButton::clicked, this, &MainMenu::loginRequested);   // connect so that when the 'login sender' 'signals clicked', 'this window' will 'call 'MainMenu::loginReequested'

    guestButton = new QPushButton("Play as Guest", this);
    mainLayout->addWidget(guestButton);
    connect(guestButton, &QPushButton::clicked, this, &MainMenu::guestPlayRequested);   //you know the story

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    settingsButton = new QPushButton(this);
    settingsButton->setIcon(QIcon::fromTheme("settings", QIcon(":/icons/gear.png")));
    settingsButton->setFixedSize(40, 40);                                           // will need an icon for settins, yea
    bottomLayout->addWidget(settingsButton);
    mainLayout->addLayout(bottomLayout);
    connect(settingsButton, &QPushButton::clicked, this, &MainMenu::settingsRequested);
}

void MainMenu::updateButtons(bool isLoggedIn) {
    if (isLoggedIn) {
        loginButton->setText("Play Game");      //if we're logged in, i want buttons to read play game and logout
        guestButton->setText("Logout");
    } else {
        loginButton->setText("Login");          //else, we can have them as is.
        guestButton->setText("Play as Guest");  // we need to update functionality as well, it is done in mainController.cpp
    }
}


