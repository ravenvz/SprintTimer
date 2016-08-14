/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef DB_HELPER_H
#define DB_HELPER_H

#include <QDebug>
#include <QHash>
#include <QObject>
#include <QSqlDatabase>
#include <QThread>
#include <QtSql>
#include <algorithm>
#include <memory>
#include <vector>


class Worker;

class DBService : public QObject {
    Q_OBJECT

public:
    DBService(QString filename);
    DBService(std::string filename);
    ~DBService();

    DBService(DBService&&) = default;
    DBService& operator=(DBService&&) = default;

    DBService(const DBService&) = default;
    DBService& operator=(const DBService&) = default;

    /* Execute single query and return it's id. */
    long long executeQuery(const QString& query);

    /* Prepare query and return it's id. */
    long long prepare(const QString& query);

    // TODO what if no query with such id
    /* Execute previously prepared query */
    void executePrepared(long long queryId);

    // TODO what if no query with such id
    /* Bind value to a placeholder for a previously prepared query
     * with given id. */
    void bindValue(long long queryId,
                   const QString& placeholder,
                   const QVariant& value);

    void transaction();

    void rollback();

    void commit();

public slots:

    void handleResults(long long queryId,
                       const std::vector<QSqlRecord>& records);

    void handleError(long long queryId, const QString& errorMessage);

signals:
    void queue(long long queryId, const QString& query);
    void queuePrepared(long long queryId);
    void results(long long queryId, const std::vector<QSqlRecord>& records);
    void error(long long queryId, const QString& errorMessage);
    void prepareQuery(long long queryId, const QString& queryStr);
    void
    bind(long long queryId, const QString& placeholder, const QVariant& value);
    void requestTransaction();
    void requestRollback();
    void requestCommit();

private:
    QThread workerThread;
    long long nextQueryId{0};
};


class Worker : public QObject {
    Q_OBJECT

public:
    Worker(const QString& filename);
    ~Worker();

public slots:
    void execute(long long queryId, const QString& query);

    void executePrepared(long long queryId);

    void prepare(long long queryId, const QString& queryStr);

    void bindValue(long long queryId,
                   const QString& placeholder,
                   const QVariant& value);

    void onTransactionRequested();

    void rollbackTransaction();

    void onCommitRequested();

signals:
    void results(long long queryId, const std::vector<QSqlRecord>& records);

    void error(long long queryId, const QString& errorMessage);

private:
    QString filename;
    QSqlDatabase db;
    bool inTransaction{false};
    QHash<long long, QSqlQuery> preparedQueries;

    bool createDatabase();

    bool createSchema();

    bool createDbConnection();

    bool activateForeignKeys();

    // void migrate();
};

#endif // DB_HELPER_H
