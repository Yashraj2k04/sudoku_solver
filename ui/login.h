// we make a login page for now, only handling status by toggling a variable for now
// will implement the actual login check later once i set up a basic db

#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>

class Login : public QWidget{
    Q_OBJECT

    public:
        explicit Login(QWidget* parent = nullptr);

    signals:
        void loginSuccessful();             //signal to be emitted upon some action (a function to be called)
        void backRequested();

    private slots:
        void handleLogin();                 //the function that will handle the signal 
        void handleBack();

    private:
        QLineEdit *usernameField;
        QLineEdit *passwordField;
        QPushButton *signInButton;
        QPushButton *signUpButton;
        QPushButton *backButton;

        void setupUI();                        //stack the widgets, set up the page 
        void setupConnections();                //
    
};





#endif