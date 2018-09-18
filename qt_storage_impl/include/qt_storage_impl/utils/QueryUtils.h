#ifndef QUERYUTILS_H_145ABLDE
#define QUERYUTILS_H_145ABLDE

#include <QSqlQuery>

namespace sprint_timer::storage::qt_storage_impl {

void tryExecute(QSqlQuery& query, const QString& queryStr);

} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: QUERYUTILS_H_145ABLDE */
