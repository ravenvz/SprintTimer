#ifndef DB_HELPER_H
#define DB_HELPER_H

#include <QtSql>
#include <QSqlDatabase>
#include <vector>
#include <algorithm>
#include "core/entities/TodoItem.h"
#include "core/entities/Pomodoro.h"
#include <QDebug>


class DBService {
public:
    DBService(QString filename);
    ~DBService();

    DBService(DBService&&) = default;
    DBService& operator=(DBService&&) = default;

    DBService(const DBService&) = default;
    DBService& operator=(const DBService&) = default;

    bool createDatabase();

    bool createSchema();

    bool createDbConnection();

    bool activateForeignKeys();

private:
    QString filename;
    QSqlDatabase db;

    bool execAndCheck(QSqlQuery& query, const QString& queryStr);
};


// bool createDatabase(QSqlDatabase& db, QString& filename);
//
// bool createSchema();
//
// bool createDbConnection();
//
// bool activateForeignKeys();
//

#endif // DB_HELPER_H
