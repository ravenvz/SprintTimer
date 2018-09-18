#include "qt_storage_impl/migrations/Migration_v4.h"
#include "qt_storage_impl/utils/QueryUtils.h"

namespace sprint_timer::storage::qt_storage_impl {

void Migration_v4::run(QSqlDatabase& database) const
{
    QSqlQuery query{database};
    tryExecute(query, "DROP TRIGGER on_todo_tag_delete");
    tryExecute(query,
               "CREATE TRIGGER clean_orphaned_tag"
               " AFTER DELETE ON task"
               " BEGIN DELETE FROM tag"
               " WHERE id = OLD.tag_id AND"
               "(SELECT count(*) FROM tasktag"
               " WHERE tag_id = OLD.tag_id) = 0;"
               "END;");
}

} // namespace sprint_timer::storage::qt_storage_impl
