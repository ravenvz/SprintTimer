#include "qt_storage_impl/utils/QueryUtils.h"
#include "qt_storage_impl/QueryError.h"

namespace sprint_timer::storage::qt_storage_impl {

void tryExecute(QSqlQuery& query, const QString& queryStr)
{
    if (!query.exec(queryStr))
        throw QueryError{"Error executing query", query};
}

} // namespace sprint_timer::storage::qt_storage_impl
