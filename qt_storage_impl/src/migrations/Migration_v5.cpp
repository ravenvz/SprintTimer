#include "qt_storage_impl/migrations/Migration_v5.h"
#include "qt_storage_impl/utils/QueryUtils.h"

namespace sprint_timer::storage::qt_storage_impl {

void Migration_v5::run(QSqlDatabase& database) const
{
    QSqlQuery query{database};
    tryExecute(query, "DROP TRIGGER instead_sprint_view_insert");
    tryExecute(query, "drop trigger delete_from_sprint_view");
    tryExecute(query,
               "CREATE TRIGGER delete_from_sprint_view"
               " INSTEAD OF DELETE ON sprint_view"
               " BEGIN DELETE FROM sprint"
               " WHERE uuid = OLD.uuid;"
               "UPDATE task SET actual_cost = actual_cost - 1"
               " WHERE uuid = OLD.task_uuid;"
               " END;");
    tryExecute(query,
               "CREATE TRIGGER instead_sprint_view_insert"
               " INSTEAD OF INSERT ON sprint_view"
               " BEGIN INSERT INTO sprint"
               " (task_uuid, start_time, finish_time, uuid)"
               " SELECT"
               " NEW.task_uuid, NEW.start_time, NEW.finish_time, NEW.uuid;"
               "UPDATE task SET actual_cost = actual_cost + 1"
               " WHERE uuid = NEW.task_uuid;"
               "END;");
}

} // namespace sprint_timer::storage::qt_storage_impl
