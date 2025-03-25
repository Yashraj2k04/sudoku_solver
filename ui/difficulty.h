#ifndef DIFFICULTY_H        //include guard, just like #pragma once

#define DIFFICULTY_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

class Difficulty : public QWidget{
    Q_OBJECT

    public:
        explicit Difficulty(QWidget * parent = nullptr);        // explicit : makes so a Difficulty object is not made when passing a qwidget* anywhere
                                                                // rest is a construct with a default argument as you can see
    signals:
        void easySelected(int hints);                                    //signals are basically functions only.
        void mediumSelected(int hints);
        void hardSelected(int hints);
        void backToMainMenu(int hints);

        void difficultySelected(int hints);


    private:
        QPushButton *easyButton;
        QPushButton *mediumButton;
        QPushButton *hardButton;
        QPushButton *backButton;

        void setupUI();
        void setupConnections();
};
#endif