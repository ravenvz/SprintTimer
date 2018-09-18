#include "qt_storage_impl/migrations/Migration_v3.h"
#include "qt_storage_impl/utils/QueryUtils.h"

namespace sprint_timer::storage::qt_storage_impl {

void Migration_v3::run(QSqlDatabase& database) const
{
    QSqlQuery query{database};
    tryExecute(query, "DROP TABLE calendar;");
}

} // namespace sprint_timer::storage::qt_storage_impl
