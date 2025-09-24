#include <QApplication>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "main_controller.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyleSheet("QWidget { background-color:white; }");

    // Load the configuration file
    QFile configFile("../config.json");
    if (!configFile.open(QIODevice::ReadOnly)) {
        qCritical() << "Could not open config file.";
        return -1;
    }

    QJsonDocument configDoc = QJsonDocument::fromJson(configFile.readAll());
    QJsonObject config = configDoc.object();

    QString hostName = config["DB_HOST"].toString();
    int port = config["DB_PORT"].toInt();
    QString dbName = config["DB_NAME"].toString();
    QString userName = config["DB_USERNAME"].toString();
    QString password = config["DB_PASSWORD"].toString();

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName(hostName);
    db.setPort(port);
    db.setDatabaseName(dbName);
    db.setUserName(userName);
    db.setPassword(password);

    if (!db.open()) {
        qCritical() << "Failed to connect to MySQL:" << db.lastError().text();
        return -1;
    }
    qDebug() << "MySQL connected successfully.";

    MainController controller;
    controller.show();
    return app.exec();
}
