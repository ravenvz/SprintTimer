#include "db_helper.h"

void createDatabase(QSqlDatabase& db, QString& filename) {
    db.setDatabaseName(filename);
    db.open();
    QSqlQuery query;
    query.exec("pragma foreign_keys = on");
    query.exec("create table pomodoro "
            "(id integer primary key autoincrement, "
            "name varchar, "
            "start_time datetime, "
            "finish_time datetime)");
    query.exec("create table tag "
            "(id integer primary key autoincrement, "
            "name varchar)");
    query.exec("create table todo_item "
            "(id integer primary key autoincrement, "
            "name varchar unique, "
            "estimated_pomodoros integer, "
            "spent_pomodoros integer, "
            "completed boolean, "
            "priority integer, "
            "last_modified datetime)");
    query.exec("create table todotag "
            "(id integer primary key autoincrement, "
            "tag_id integer not null, "
            "todo_id integer not null, "
            "foreign key(tag_id) references tag(id) on delete cascade on update cascade, "
            "foreign key(todo_id) references todo_item(id) on delete cascade on update cascade)");

    // qDebug() << query.lastError() << " this error";
    // db.close();
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
