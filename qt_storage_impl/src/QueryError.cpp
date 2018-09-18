#include "qt_storage_impl/QueryError.h"
#include <QSqlError>

namespace sprint_timer::storage::qt_storage_impl {

QueryError::QueryError(const std::string& description, const QSqlQuery& query)
    : std::runtime_error{description}
    , query{query.lastQuery().toStdString()}
    , lastError{query.lastError().text().toStdString()}
{
}


std::string QueryError::queryText() const { return query; }

std::string QueryError::queryError() const { return lastError; }


} // namespace sprint_timer::storage::qt_storage_impl
