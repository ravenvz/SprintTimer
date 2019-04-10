/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "qt_storage_impl/DBService.h"
#include "qt_storage_impl/DatabaseError.h"
#include "qt_storage_impl/DatabaseInitializer.h"
#include "qt_storage_impl/QueryError.h"

#include <iostream>

namespace sprint_timer::storage::qt_storage_impl {

DBService::DBService(const QString& filename)
{
    qRegisterMetaType<std::vector<QSqlRecord>>("std::vector<QSqlRecord>");

    // TODO exception should be presented to the user in better way, and,
    // probably, not here
    try {
        prepareDatabase(filename);
    }
    catch (const QueryError& err) {
        std::cerr << "Attempt to query database resulted in exception: "
                  << err.what() << '\n'
                  << "Query: \n"
                  << err.queryText() << '\n'
                  << "Error: \n"
                  << err.queryError() << '\n';
        throw;
    }
    catch (const DatabaseError& err) {
        std::cerr << "Database error: " << err.what() << '\n'
                  << "Error:\n"
                  << err.error() << '\n';
        throw;
    }
    auto workerPtr = std::make_unique<Worker>(filename);
    connect(&workerThread, &QThread::started, workerPtr.get(), &Worker::init);
    connect(&workerThread, &QThread::finished, workerPtr.get(), &QObject::deleteLater);
    connect(this, &DBService::queue, workerPtr.get(), &Worker::execute);
    connect(this, &DBService::queuePrepared, workerPtr.get(), &Worker::executePrepared);
    connect(workerPtr.get(), &Worker::results, this, &DBService::handleResults);
    connect(workerPtr.get(), &Worker::error, this, &DBService::handleError);
    connect(this, &DBService::prepareQuery, workerPtr.get(), &Worker::prepare);
    connect(this, &DBService::bind, workerPtr.get(), &Worker::bindValue);
    connect(this,
            &DBService::requestTransaction,
            workerPtr.get(),
            &Worker::onTransactionRequested);
    connect(this,
            &DBService::requestRollback,
            workerPtr.get(),
            &Worker::rollbackTransaction);
    connect(
        this, &DBService::requestCommit, workerPtr.get(), &Worker::onCommitRequested);
	auto* worker = workerPtr.release();
    worker->moveToThread(&workerThread);
    workerThread.start();
}

DBService::DBService(const std::string& filename)
    : DBService(QString::fromStdString(filename))
{
}

DBService::~DBService()
{
    workerThread.quit();
    workerThread.wait();
}

void DBService::prepareDatabase(const QString& filename) const
{
    DatabaseInitializer{filename};
}

qint64 DBService::execute(const QString& query)
{
    emit queue(nextQueryId, query);
    return nextQueryId++;
}

qint64 DBService::prepare(const QString& query)
{
    emit prepareQuery(nextQueryId, query);
    return nextQueryId++;
}

void DBService::executePrepared(qint64 queryId) { emit queuePrepared(queryId); }

void DBService::handleResults(qint64 queryId,
                              const std::vector<QSqlRecord>& records)
{
    emit results(queryId, records);
}

void DBService::handleError(qint64 queryId, const QString& errorMessage)
{
    qDebug() << queryId;
    qDebug() << errorMessage;
    emit error(queryId, errorMessage);
}

void DBService::bindValue(qint64 queryId,
                          const QString& placeholder,
                          const QVariant& value)
{
    emit bind(queryId, placeholder, value);
}

void DBService::transaction() { emit requestTransaction(); }

void DBService::rollback() { emit requestRollback(); }

void DBService::commit() { emit requestCommit(); }

Worker::Worker(QString filename)
    : filename{std::move(filename)}
{
}

void Worker::init()
{
    connection = std::make_unique<ConnectionGuard>(filename, connectionName);
    if (!openConnection())
        throw std::runtime_error("Unable to connect with database");
}

bool Worker::openConnection()
{
    // if (!QSqlDatabase::database(connectionName).open()) {
    //     qCritical() << "Worker failed to open database";
    //     return false;
    // }
    return setPragmas();
}

bool Worker::setPragmas()
{
    QSqlQuery query{QSqlDatabase::database(connectionName)};
    return query.exec("PRAGMA foreign_keys = ON");
}

void Worker::execute(qint64 queryId, const QString& query)
{
    QSqlQuery q{QSqlDatabase::database(connectionName)};
    if (!q.exec(query)) {
        QString errormsg
            = QString("%1 %2").arg(q.lastQuery()).arg(q.lastError().text());
        qDebug() << errormsg;
        emit error(queryId, errormsg);
        if (inTransaction) {
            rollbackTransaction();
        }
    }
    else {
        std::vector<QSqlRecord> recs;
        while (q.next()) {
            recs.push_back(q.record());
        }
        emit results(queryId, recs);
    }
}

void Worker::executePrepared(qint64 queryId)
{
    auto found = preparedQueries.find(queryId);
    if (found == preparedQueries.end()) {
        emit error(queryId, "Cannot find query with id");
        return;
    }
    QSqlQuery query = found.value();
    if (!query.exec()) {
        QString errormsg = QString("%1 %2")
                               .arg(query.lastQuery())
                               .arg(query.lastError().text());
        if (inTransaction) {
            rollbackTransaction();
        }
        emit error(queryId, errormsg);
    }
    else {
        // qDebug() << query.lastQuery();
        std::vector<QSqlRecord> recs;
        while (query.next()) {
            recs.push_back(query.record());
        }
        emit results(queryId, recs);
    }
}

void Worker::prepare(qint64 queryId, const QString& queryStr)
{
    QSqlQuery query{QSqlDatabase::database(connectionName)};
    query.prepare(queryStr);
    preparedQueries.insert(queryId, query);
}

void Worker::bindValue(qint64 queryId,
                       const QString& placeholder,
                       const QVariant& value)
{
    if (auto found = preparedQueries.find(queryId);
        found != preparedQueries.end())
        found.value().bindValue(placeholder, value);
    else
        emit error(queryId,
                   "Attempting to bind value to query that was not prepared");
}

void Worker::onTransactionRequested()
{
    inTransaction = QSqlDatabase::database(connectionName).transaction();
    if (!inTransaction) {
        emit error(-1, "Transaction request failed");
    }
}

void Worker::rollbackTransaction()
{
    if (!QSqlDatabase::database(connectionName).rollback()) {
        emit error(-1, "Transaction rollback failed");
    }
    inTransaction = false;
}

void Worker::onCommitRequested()
{
    if (!QSqlDatabase::database(connectionName).commit()) {
        emit error(-1, "Transaction commit failed");
    }
    inTransaction = false;
}

} // namespace sprint_timer::storage::qt_storage_impl
