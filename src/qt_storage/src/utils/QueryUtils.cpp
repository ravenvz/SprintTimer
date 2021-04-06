#include "qt_storage/utils/QueryUtils.h"
#include "qt_storage/QueryError.h"
#include <QSqlDatabase>
#include <QSqlRecord>

#include <iostream>

namespace sprint_timer::storage::qt_storage {

void tryExecute(QSqlQuery& query, const QString& queryStr)
{
    if (!query.exec(queryStr)) {
        throw QueryError{"Error executing query", query};
    }
}

void tryExecute(QSqlQuery& query)
{
    if (!query.exec()) {
        throw qt_storage::QueryError{"Error executing query", query};
    }
}

QSqlQuery tryPrepare(const QString& connectionName, const QString& queryStr)
{
    QSqlQuery query{QSqlDatabase::database(connectionName)};
    if (!query.prepare(queryStr)) {
        throw qt_storage::QueryError{"Error preparing query", query};
    }
    return query;
}

std::vector<QSqlRecord> tryFetchAll(QSqlQuery& query, const QString& queryStr)
{
    if (!query.exec(queryStr)) {
        throw QueryError{"Error executing query", query};
    }
    return copyAllRecords(query);
}

std::vector<QSqlRecord> tryFetchAll(QSqlQuery& query)
{
    if (!query.exec()) {
        throw QueryError{"Error executing query", query};
    }
    return copyAllRecords(query);
}

std::vector<QSqlRecord> copyAllRecords(QSqlQuery& query)
{
    std::vector<QSqlRecord> records;
    // It is not guaranteed that the size will be determined, or driver has this
    // feature
    if (query.size() > 0) {
        records.reserve(static_cast<size_t>(query.size()));
    }
    while (query.next()) {
        records.push_back(query.record());
    }
    query.finish();
    return records;
}

} // namespace sprint_timer::storage::qt_storage
