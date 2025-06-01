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
    setupLoginConnections();

    stackedWidget->setCurrentWidget(mainMenu);

}


// main window connections
void MainController::setupConnections() {
    connect(mainMenu, &MainMenu::guestPlayRequested, [this]() {
        if (loginStatus) {                  //kinda tuff. okay, so dont get confused by the 'guestPlayRequested' as it is only the name of the signal
                                            // if loginStatus is true, it will read 'Logout' in game. so when we press 'Logout', it sets the loginStatus
                                            // to be false, and updates what the buttons read now by calling the updateButtons in mainMenu.
            loginStatus = false;
            mainMenu->updateButtons(false); // Reset button text when logged out
            stackedWidget->setCurrentWidget(mainMenu); // Go back to main menu
        } else {
            stackedWidget->setCurrentWidget(difficultyScreen); // Guest play
        }
    });

    connect(mainMenu, &MainMenu::loginRequested, [this]() {
        if (loginStatus) {
            stackedWidget->setCurrentWidget(difficultyScreen); // Already logged in, go to difficulty
        } else {
            stackedWidget->setCurrentWidget(loginScreen); // Not logged in, show login screen
        }
    });
    connect(mainWindow, &MainWindow::backToDifficultyScreen, [this](){
        stackedWidget->setCurrentWidget(difficultyScreen);
    });
}


//login screen connections
void MainController::setupLoginConnections() {
    connect(loginScreen, &Login::backRequested, [this]() {      //we connect login screen l
        stackedWidget->setCurrentWidget(mainMenu);
    });

    connect(loginScreen, &Login::loginSuccessful, [this](const QString &username) {
        loginStatus = true;
        mainMenu->updateButtons(true); // Update button text after login

        currentUsernname = username;
        mainWindow->setUsername(currentUsernname);
        stackedWidget->setCurrentWidget(mainMenu);
    });
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


