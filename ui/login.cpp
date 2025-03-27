#include "login.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

Login::Login(QWidget* parent) : QWidget(parent){

    setupUI();
    setupConnections();

}
void Login::setupUI(){

    if (this->layout()) {
        delete this->layout();  // Remove existing layout to avoid conflicts
    }
    //initialize fields.
    usernameField = new QLineEdit(this);

    passwordField = new QLineEdit(this);
    // passwordField->setEchoMode(QLineEdit::Password);  // Hide password input


    //initialize buttons
    signInButton = new QPushButton("Sign In",this);
    signUpButton = new QPushButton("Sign Up",this);

    //init layouts, add thw widgets to layouts
    QVBoxLayout *layout = new QVBoxLayout(this);    //main layout that attatches itself to main widget window. thus, we pass 'this'

    layout->addWidget(usernameField);
    layout->addWidget(passwordField);


    QHBoxLayout *buttonsLayout = new QHBoxLayout(); //not main layout, since vbox is main. we dont pass this here. we dont do that in nested, instead,
    layout->addLayout(buttonsLayout);               //we add hbox layout to vbox layout. so, when vbox is destroyed (when widget is destroyed), hbox is destroyed with it.
    buttonsLayout->addWidget(signInButton);
    buttonsLayout->addWidget(signUpButton);

}
void Login::handleLogin(){

}
void Login::setupConnections(){

}