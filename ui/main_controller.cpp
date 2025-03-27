// this will handle the screen change between buttons like login, settings, play as guest etc

#include "main_controller.h"

#include <QVBoxLayout>

MainController::MainController(QWidget *parent) : QWidget(parent), loginStatus(false) {
    stackedWidget = new QStackedWidget(this);

    mainMenu = new MainMenu(this);                      //pointer to main menu so we can switch to that 'screen' (the title screen)
    mainWindow = new MainWindow(this);                  //pointer to main window so we can switch to that 'screen' (the board)
    difficultyScreen = new Difficulty(this);            //pointer to difficulty screen

    loginScreen = new Login(this);                      //pointer to login screen

    stackedWidget->addWidget(mainMenu);                 // adding those screens to our stacked widget so we can show the one we want whenever
    stackedWidget->addWidget(mainWindow);
    stackedWidget->addWidget(difficultyScreen);
    stackedWidget->addWidget(loginScreen);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(stackedWidget);
    setLayout(layout);

    setupConnections();
    setupDifficultyConnections();

    stackedWidget->setCurrentWidget(mainMenu);

}


// main window connections
void MainController::setupConnections() {
    connect(mainMenu, &MainMenu::guestPlayRequested, [this]() {
        stackedWidget->setCurrentWidget(difficultyScreen);  // Show difficulty screen first
    });

    connect(mainMenu, &MainMenu::loginRequested, [this](){
        stackedWidget->setCurrentWidget(loginScreen);
    } );

}



// difficulty screen connections
void MainController::setupDifficultyConnections() {
    connect(difficultyScreen, &Difficulty::difficultySelected, mainWindow, &MainWindow::setHints);
    connect(difficultyScreen, &Difficulty::difficultySelected, [this]() {
        stackedWidget->setCurrentWidget(mainWindow);  // Show the game board after difficulty selection
    });
    connect(difficultyScreen, &Difficulty::backToMainMenu, [this]() {
        stackedWidget->setCurrentWidget(mainMenu);
    });    

}


