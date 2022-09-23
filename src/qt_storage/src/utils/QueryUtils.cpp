#include "qt_storage/utils/QueryUtils.h"
#include "qt_storage/QueryError.h"
#include <QSqlDatabase>
#include <QSqlRecord>

#include <iostream>

namespace sprint_timer::storage::qt_storage {

auto tryExecute(QSqlQuery& query, const QString& queryStr) -> void
{
    try {
        if (!query.exec(queryStr)) {
            throw QueryError{"Error executing query", query};
        }
    }
    catch (QueryError& err) {
        std::cerr << err.queryText() << std::endl;
        std::cerr << err.queryError() << std::endl;
        throw;
    }
}

auto tryExecute(QSqlQuery& query) -> void
{
    try {
        if (!query.exec()) {
            throw qt_storage::QueryError{"Error executing query", query};
        }
    }
    catch (QueryError& err) {
        std::cerr << err.queryText() << std::endl;
        std::cerr << err.queryError() << std::endl;
        throw;
    }
}

auto tryPrepare(const QString& connectionName, const QString& queryStr)
    -> QSqlQuery
{
    QSqlQuery query{QSqlDatabase::database(connectionName)};
    try {
        if (!query.prepare(queryStr)) {
            throw qt_storage::QueryError{"Error preparing query", query};
        }
        return query;
    }
    catch (QueryError& err) {
        std::cerr << err.queryText() << std::endl;
        std::cerr << err.queryError() << std::endl;
        throw;
    }
}

auto tryFetchAll(QSqlQuery& query, const QString& queryStr)
    -> std::vector<QSqlRecord>
{
    if (!query.exec(queryStr)) {
        throw QueryError{"Error executing query", query};
    }
    return copyAllRecords(query);
}

auto tryFetchAll(QSqlQuery& query) -> std::vector<QSqlRecord>
{
    if (!query.exec()) {
        throw QueryError{"Error executing query", query};
    }
    return copyAllRecords(query);
}

auto copyAllRecords(QSqlQuery& query) -> std::vector<QSqlRecord>
{
    std::vector<QSqlRecord> records;
    // It is not guaranteed that the size will be determined or that driver has
    // this feature
    if (query.size() > 0) {
        records.reserve(static_cast<size_t>(query.size()));
    }
    while (query.next()) {
        records.push_back(query.record());
    }
    query.finish();
    return records;
}

auto maybeString(const QSqlRecord& record, int column)
    -> std::optional<std::string>
{
    if (auto val = record.value(column); not val.isNull()) {
        return val.toString().toStdString();
    }
    return std::nullopt;
}

auto maybeInt(const QSqlRecord& record, int column) -> std::optional<int>
{
    if (auto val = record.value(column); not val.isNull()) {
        return val.toInt();
    }
    return std::nullopt;
}

auto maybeBool(const QSqlRecord& record, int column) -> std::optional<bool>
{
    if (auto val = record.value(column); not val.isNull()) {
        return val.toBool();
    }
    return std::nullopt;
}

auto maybeDateTime(const QSqlRecord& record, int column)
    -> std::optional<QDateTime>
{
    if (auto val = record.value(column); !val.isNull()) {
        return val.toDateTime();
    }
    return std::nullopt;
}

} // namespace sprint_timer::storage::qt_storage
