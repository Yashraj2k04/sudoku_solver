// header, this defines the structure of the game screen that is shown to the player once they start the game

#ifndef MAIN_MENU_H             //include guard
#define MAIN_MENU_H

#include <QWidget>              //main widget of our main_menu screen
#include <QPushButton>          //button widget for login, setting, play as guest etc
#include <QLabel>               // label for sreen title


class MainMenu : public QWidget{
    Q_OBJECT                    // Q object enables widgets to communicate with eachother.

    public:
        explicit MainMenu(QWidget *parent = nullptr);

    signals:                    //signals we'll be sending on button trigger (basically calling them)
        void loginRequested();
        void guestPlayRequested();
        void settingsRequested();

    private:
        QLabel * titleLabel;
        QPushButton *loginButton;
        QPushButton *guestButton;
        QPushButton *settingsButton;

        void setupUI();


};


#endif