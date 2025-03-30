#include "login.h"
#include <QVBoxLayout>
#include <QHBoxLayout>

Login::Login(QWidget* parent) : QWidget(parent){
    setupUI();
    setupConnections();
}
void Login::setupUI() {
    if (this->layout()) {
        delete this->layout();
    }
    

    // Initialize fields
    usernameField = new QLineEdit(this);
    passwordField = new QLineEdit(this);
    passwordField->setEchoMode(QLineEdit::Password);
    usernameField->setFixedHeight(usernameField->height() + 25);
    passwordField->setFixedHeight(passwordField->height() + 25);
    usernameField->setStyleSheet("font-size: 22px; padding: 5px;");
    passwordField->setStyleSheet("font-size: 22px; padding: 5px;");

    usernameField->setPlaceholderText("username");
    passwordField->setPlaceholderText("password");


    // Initialize buttons
    signInButton = new QPushButton("Sign In", this);
    signUpButton = new QPushButton("Sign Up", this);
    backButton = new QPushButton("Back", this);

    signInButton->setCursor(Qt::PointingHandCursor);
    signUpButton->setCursor(Qt::PointingHandCursor);
    backButton->setCursor(Qt::PointingHandCursor); 

    // Styling for buttons
    signInButton->setStyleSheet(
        "QPushButton {"
        "background-color: #4CAF50;"
        "color: white;"
        "padding: 10px;"
        "font-size: 16px;"
        "border-radius: 20px;"
        "}"
        "QPushButton:hover {"
        "background-color: #388E3C;"
        "}"
    );
    signUpButton->setStyleSheet(
        "QPushButton {"
        "background-color: #808080;"
        "color: white;"
        "padding: 10px;"
        "font-size: 16px;"
        "border-radius: 20px;"
        "}"
        "QPushButton:hover {"
        "background-color: #606060;"
        "}"
    );
    signInButton->setFixedHeight(signInButton->height() + 20);
    signUpButton->setFixedHeight(signUpButton->height() + 20);

    backButton->setStyleSheet("background-color:transparent;"
                              "color:#000000;"
                              "font-size:15px;"
                              "border: none;"
                              "padding: 5px;");

    

    // Styling for back button
    backButton->setFixedSize(60, 30);

    // Top bar layout (back button at the top-left)
    QHBoxLayout *topBarLayout = new QHBoxLayout();
    topBarLayout->setContentsMargins(10, 10, 0, 0);
    topBarLayout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    topBarLayout->addWidget(backButton);

    // VBox for input fields + buttons
    QVBoxLayout *inputContainer = new QVBoxLayout();
    inputContainer->setSpacing(20);  // 5px margin between username & password fields
    inputContainer->setContentsMargins(0, 0, 0, 0);

    inputContainer->addWidget(usernameField);
    inputContainer->addWidget(passwordField);

    // HBox for buttons
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    buttonsLayout->addWidget(signInButton);
    buttonsLayout->addWidget(signUpButton);
    inputContainer->addLayout(buttonsLayout);

    // Wrapper widget for dynamic sizing
    QWidget *inputWrapper = new QWidget(this);
    inputWrapper->setLayout(inputContainer);
    inputWrapper->setMinimumWidth(300);
    inputWrapper->setMaximumWidth(500);  // Max width limit
    inputWrapper->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // Main layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addLayout(topBarLayout, 1);  // Top bar (10% height)
    layout->addStretch(1);               // Spacer before inputs

    // Dynamic centering with max width limit
    QHBoxLayout *hCenteringLayout = new QHBoxLayout();
    hCenteringLayout->addStretch(1);
    hCenteringLayout->addWidget(inputWrapper, 1);
    hCenteringLayout->addStretch(1);

    layout->addLayout(hCenteringLayout, 7); // 70% height for input section
    layout->addStretch(2);                  // 20% bottom padding

    setLayout(layout);
}



void Login::handleLogin(){
    emit loginSuccessful();
}

void Login::handleBack(){
    emit backRequested();
}

void Login::setupConnections(){
    connect(signInButton, &QPushButton::clicked, this, &Login::handleLogin);
    connect(backButton, &QPushButton::clicked, this, &Login::handleBack);
}
