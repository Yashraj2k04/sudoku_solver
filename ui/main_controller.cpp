// this will handle the screen change between buttons like login, settings, play as guest etc




#include "main_controller.h"
#include <QVBoxLayout>

MainController::MainController(QWidget *parent) : QWidget(parent) {
    stackedWidget = new QStackedWidget(this);

    mainMenu = new MainMenu(this);                      //pointer to main menu so we can swithc to that 'screen'    (the title screen)
    mainWindow = new MainWindow(this);                  //pointer to main window so we can swithc to that 'screen' (the board)

    stackedWidget->addWidget(mainMenu);                 // adding those screens to our stacked widget so we can show the one we want whenever
    stackedWidget->addWidget(mainWindow);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(stackedWidget);
    setLayout(layout);

    setupConnections();

    stackedWidget->setCurrentWidget(mainMenu);
}

void MainController::setupConnections() {                           // TW: lambda function incoming.
    connect(mainMenu, &MainMenu::guestPlayRequested, [this]() {                 // takes 'this' (window) as a parameter from the surrounding scope and copies it
        stackedWidget->setCurrentWidget(mainWindow);                            //basically, when 'target mainMenu (window) signals' 'guestPlayRequested', thenmake 'this(current window we are in, this is mainMenu)
    });                                                                         // do -> set stackedWidget -> setCurrentWidget the mainWindow (our game board)
}
