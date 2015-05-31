#include <QtSql>

void createDatabase(QSqlDatabase& db, QString& filename) {
    db.setDatabaseName(filename);
    db.open();
    QSqlQuery query;
    query.exec("create table pomodoro "
            "(id integer primary key autoincrement, "
            "name varchar(50), "
            "start_time datetime, "
            "finish_time datetime)");
}


bool connectToDatabase() {
    QString filename = "db/pomodoro.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QFile database(filename);
    if (!database.exists()) {
        qDebug() << "Database not found. Creating...";
        createDatabase(db, filename);
    } else {
        qDebug() << "Database found. Opening...";
        db.setDatabaseName(filename);
        db.open();
    }
    return db.isOpen();
}
