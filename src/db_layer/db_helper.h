#ifndef DB_HELPER_H
#define DB_HELPER_H

#include <QtSql>
#include <QSqlDatabase>
#include <vector>
#include <algorithm>
#include "core/entities/TodoItem.h"
#include "core/entities/Pomodoro.h"
#include <QDebug>


bool createDatabase(QSqlDatabase& db, QString& filename);

bool createSchema();

bool createDbConnection();

bool activateForeignKeys();


#endif // DB_HELPER_H
