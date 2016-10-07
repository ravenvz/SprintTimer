/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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
#include <include/qt_storage_impl/Database.h>
#include "qt_storage_impl/DBService.h"


DBService::DBService(QString filename)
{
    // TODO handle exception
    prepareDatabase(filename);
    Worker* worker = new Worker(filename);
    worker->moveToThread(&workerThread);
    qRegisterMetaType<std::vector<QSqlRecord>>("std::vector<QSqlRecord>");
    connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &DBService::queue, worker, &Worker::execute);
    connect(this, &DBService::queuePrepared, worker, &Worker::executePrepared);
    connect(worker, &Worker::results, this, &DBService::handleResults);
    connect(worker, &Worker::error, this, &DBService::handleError);
    connect(this, &DBService::prepareQuery, worker, &Worker::prepare);
    connect(this, &DBService::bind, worker, &Worker::bindValue);
    connect(this,
            &DBService::requestTransaction,
            worker,
            &Worker::onTransactionRequested);
    connect(this,
            &DBService::requestRollback,
            worker,
            &Worker::rollbackTransaction);
    connect(
        this, &DBService::requestCommit, worker, &Worker::onCommitRequested);
    workerThread.start();
}

DBService::DBService(std::string filename)
    : DBService(QString::fromStdString(std::move(filename)))
{
}


DBService::~DBService()
{
    workerThread.quit();
    workerThread.wait();
}

void DBService::prepareDatabase(const QString& filename) const {
    auto db = Database(filename);
}


long long DBService::executeQuery(const QString& query)
{
    emit queue(nextQueryId, query);
    return nextQueryId++;
}


long long DBService::prepare(const QString& query)
{
    emit prepareQuery(nextQueryId, query);
    return nextQueryId++;
}


void DBService::executePrepared(long long queryId)
{
    emit queuePrepared(queryId);
}


void DBService::handleResults(long long queryId,
                              const std::vector<QSqlRecord>& records)
{
    emit results(queryId, records);
}


void DBService::handleError(long long queryId, const QString& errorMessage)
{
    qDebug() << queryId;
    qDebug() << errorMessage;
    emit error(queryId, errorMessage);
}


void DBService::bindValue(long long queryId,
                          const QString& placeholder,
                          const QVariant& value)
{
    emit bind(queryId, placeholder, value);
}


void DBService::transaction() { emit requestTransaction(); }


void DBService::rollback() { emit requestRollback(); }


void DBService::commit() { emit requestCommit(); }


Worker::Worker(const QString& filename)
    : filename{std::move(filename)}
    , db{QSqlDatabase::addDatabase("QSQLITE")}
{
    if (!openConnection())
        throw std::runtime_error("Unable to create database");
}


Worker::~Worker()
{
    db.close();
    QSqlDatabase::removeDatabase("QSQLITE");
}


void Worker::execute(long long queryId, const QString& query)
{
    QSqlQuery q;
    if (!q.exec(query)) {
        QString errormsg
            = QString("%1 %2").arg(q.lastQuery()).arg(q.lastError().text());
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

void Worker::executePrepared(long long queryId)
{
    // TODO handle missing id case
    QSqlQuery query = preparedQueries.value(queryId);
    if (!query.exec()) {
        QString errormsg = QString("%1 %2")
                               .arg(query.lastQuery())
                               .arg(query.lastError().text());
        emit error(queryId, errormsg);
        if (inTransaction) {
            rollbackTransaction();
        }
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


void Worker::prepare(long long queryId, const QString& queryStr)
{
    QSqlQuery query;
    query.prepare(queryStr);
    preparedQueries.insert(queryId, query);
}


void Worker::bindValue(long long queryId,
                       const QString& placeholder,
                       const QVariant& value)
{
    preparedQueries[queryId].bindValue(placeholder, value);
}


void Worker::onTransactionRequested()
{
    inTransaction = db.transaction();
    if (!inTransaction) {
        emit error(-1, "Transaction request failed");
    }
}


void Worker::rollbackTransaction()
{
    if (!db.rollback()) {
        emit error(-1, "Transaction rollback failed");
    }
    inTransaction = false;
}


void Worker::onCommitRequested()
{
    if (!db.commit()) {
        emit error(-1, "Transaction commit failed");
    }
    inTransaction = false;
}


bool Worker::openConnection()
{
    db.setDatabaseName(filename);

    if (!db.open()) {
        qCritical() << "Worker failed to open database";
        return false;
    }
    setPragmas();
    return true;
}

bool Worker::setPragmas()
{
    QSqlQuery query;
    return query.exec("PRAGMA foreign_keys = ON");
}

