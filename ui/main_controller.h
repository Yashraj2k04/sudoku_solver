    // header, defines the structure of  how this will handle the screen change between buttons like login, settings, play as guest etc



    #ifndef MAIN_CONTROLLER_H
    #define MAIN_CONTROLLER_H

    #include <QStackedWidget>               //stacked widgets babyyyyy. one widget over the other. helps us change 'screens' like from login to the board itself in the same window
    #include <QWidget>
    #include "main_menu.h"
    #include "main_window.h"
    #include "difficulty.h"
    #include "login.h"
    class MainController : public QWidget {         
        Q_OBJECT        //moc i suppose :( 

    public:
        explicit MainController(QWidget *parent = nullptr);

    private:
        QStackedWidget *stackedWidget;
        MainMenu *mainMenu;
        MainWindow *mainWindow;
        Difficulty *difficultyScreen;
        Login *loginScreen;

        void setupConnections();
        void setupDifficultyConnections();
        void setupLoginConnections();

        bool loginStatus;
        QString currentUsernname;

        
    };

    #endif 
