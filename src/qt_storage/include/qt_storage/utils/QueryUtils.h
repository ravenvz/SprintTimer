#ifndef QUERYUTILS_H_145ABLDE
#define QUERYUTILS_H_145ABLDE

#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

namespace sprint_timer::storage::qt_storage {

void tryExecute(QSqlQuery& query, const QString& queryStr);

void tryExecute(QSqlQuery& query);

QSqlQuery tryPrepare(const QString& connectionName, const QString& queryStr);

/* Executes query and fetches all records. Query is finalized. May throw
 * QueryError. */
std::vector<QSqlRecord> tryFetchAll(QSqlQuery& query, const QString& queryStr);

/* Executes query and fetches all records. Query is finalized. May throw
 * QueryError. */
std::vector<QSqlRecord> tryFetchAll(QSqlQuery& query);

std::vector<QSqlRecord> copyAllRecords(QSqlQuery& query);

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QUERYUTILS_H_145ABLDE */
