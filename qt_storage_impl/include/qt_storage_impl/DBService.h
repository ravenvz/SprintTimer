/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
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

namespace sprint_timer::storage::qt_storage_impl {

class Worker;

class DBService : public QObject {
    Q_OBJECT

public:
    explicit DBService(const QString& filename);
    explicit DBService(const std::string& filename);
    ~DBService() override;

    DBService(DBService&&) = delete;
    DBService& operator=(DBService&&) = delete;

    DBService(const DBService&) = delete;
    DBService& operator=(const DBService&) = delete;

    /* Execute single query and return it's id. */
    qint64 execute(const QString& query);

    /* Prepare query and return it's id. */
    qint64 prepare(const QString& query);

    /* Execute previously prepared query.
     * Assumes that queryId is valid. */
    void executePrepared(qint64 queryId);

    /* Bind value to a placeholder for a previously prepared query
     * with given id.
     * Assumes that queryId is valid. */
    void bindValue(qint64 queryId,
                   const QString& placeholder,
                   const QVariant& value);

    void transaction();

    void rollback();

    void commit();

public slots:

    void handleResults(qint64 queryId,
                       const std::vector<QSqlRecord>& records);

    void handleError(qint64 queryId, const QString& errorMessage);

signals:
    void queue(qint64 queryId, const QString& query);
    void queuePrepared(qint64 queryId);
    void results(qint64 queryId, const std::vector<QSqlRecord>& records);
    void error(qint64 queryId, const QString& errorMessage);
    void prepareQuery(qint64 queryId, const QString& queryStr);
    void bind(qint64 queryId, const QString& placeholder, const QVariant& value);
    void requestTransaction();
    void requestRollback();
    void requestCommit();

private:
    QThread workerThread;
    // Note that nextQueryId is not atomic or protected with locks,
    // as only one thread accessing DBService at a time. Should the
    // need of multi-thread access arise, this issue should be addressed.
    qint64 nextQueryId{0};

    void prepareDatabase(const QString& filename) const;
};


class Worker : public QObject {
    Q_OBJECT

public:
    explicit Worker(QString filename);
    ~Worker() override;
    Worker(const Worker&) = delete;
    Worker& operator=(const Worker&) = delete;
    Worker(Worker&&) = delete;
    Worker& operator=(Worker&&) = delete;

public slots:
    void init();

    void execute(qint64 queryId, const QString& query);

    void executePrepared(qint64 queryId);

    void prepare(qint64 queryId, const QString& queryStr);

    void bindValue(qint64 queryId,
                   const QString& placeholder,
                   const QVariant& value);

    void onTransactionRequested();

    void rollbackTransaction();

    void onCommitRequested();

signals:
    void results(qint64 queryId, const std::vector<QSqlRecord>& records);

    void error(qint64 queryId, const QString& errorMessage);

private:
    QString filename;
    bool inTransaction{false};
    QHash<qint64, QSqlQuery> preparedQueries;

    bool openConnection();

    bool setPragmas();

};

} // namespace sprint_timer::storage::qt_storage_impl

#endif // DB_HELPER_H
