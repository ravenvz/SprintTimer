#include "qt_storage_impl/DatabaseError.h"
#include <QSqlError>

namespace sprint_timer::storage::qt_storage_impl {

DatabaseError::DatabaseError(const std::string& description,
                             const QSqlDatabase& database)
    : std::runtime_error{description}
    , error_{database.lastError().text().toStdString()}
{
}

std::string DatabaseError::error() const { return error_; }


} // namespace sprint_timer::storage::qt_storage_impl
