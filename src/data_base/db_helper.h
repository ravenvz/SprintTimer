#ifndef DB_HELPER_H
#define DB_HELPER_H


#include <QtSql>
#include <QSqlDatabase>
#include "core/entities.h"


// void createDatabase(QSqlDatabase& db, QString& filename);
inline void createDatabase(QSqlDatabase& db, QString& filename) {
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

// bool createDbConnection();
inline bool createDbConnection() {
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


// std::shared_ptr<QSqlDatabase> getDbConnection() {
//     QString filename = "db/pomodoro.db";
//     // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
//     std::shared_ptr<QSqlDatabase> db_ptr(new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE")));
//     (*db_ptr).setDatabaseName(filename);
//     return db_ptr;
// }
//

class IDataMapper
{
public:
    IDataMapper ();
    virtual ~IDataMapper ();

private:
    /* data */
};


class PomodoroMapper
{
public:
    // PomodoroMapper ();
    // virtual ~PomodoroMapper ();

    // bool create(Pomodoro pomodoro);
        

private:
    /* data */
};

class PomodoroRepository
{
public:
    PomodoroRepository() :
        dataMapper(PomodoroMapper())
    {}
    // virtual ~PomodoroRepository ();
    QStringList getPomodorosForToday() {
        QStringList result;
        QSqlQuery query;
        query.exec("select * from pomodoro");
        while (query.next()) {
            QStringList m;
            m.append(query.value(0).toString());
            m.append(query.value(1).toString());
            m.append(query.value(2).toString());
            m.append(query.value(3).toString());
            QString r = m.join(" ");
            result.append(r);
        }
        return result;
    }

    // bool storePomodoro(Pomodoro pomodoro);

private:
    PomodoroMapper dataMapper;
};

#endif // DB_HELPER_H
