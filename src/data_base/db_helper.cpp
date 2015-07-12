#include "db_helper.h"

void createDatabase(QSqlDatabase& db, QString& filename) {
    db.setDatabaseName(filename);
    db.open();
    QSqlQuery query;
    query.exec("create table pomodoro "
            "(id integer primary key autoincrement, "
            "name varchar(50), "
            "start_time datetime, "
            "finish_time datetime)");
    db.close();
}

bool createDbConnection() {
    QString filename = "db/pomodoro.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QFile databaseFile(filename);
    if (!databaseFile.exists()) {
        qDebug() << "Database not found. Creating...";
        createDatabase(db, filename);
    } else {
        qDebug() << "Database found. Opening...";
        db.setDatabaseName(filename);
    }
    if (!db.open()) {
        return false;
    }
    return true;
}



