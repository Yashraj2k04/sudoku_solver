#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include "main_controller.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);   //args 
    app.setStyleSheet("QWidget { background-color:white; }");
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("../users.db");  // your sqlite file path

    if (!db.open()) {
        qCritical() << "Failed to open database:" << db.lastError().text();
        return -1;  // Exit if DB connection fails
    }
    qDebug() << "Database connected successfully.";
    MainController controller;
    controller.show();
    return app.exec();
}
