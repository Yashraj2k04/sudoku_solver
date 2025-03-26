#include <QApplication>
#include "main_controller.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);   //args 
    MainController controller;
    controller.show();
    return app.exec();
}
