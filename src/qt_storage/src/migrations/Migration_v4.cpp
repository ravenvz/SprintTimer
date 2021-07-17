/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "qt_storage/migrations/Migration_v4.h"
#include "qt_storage/utils/QueryUtils.h"

namespace sprint_timer::storage::qt_storage {

void Migration_v4::run(const QString& connectionName) const
{
    QSqlQuery query{QSqlDatabase::database(connectionName)};
    tryExecute(query, "DROP TRIGGER on_todo_tag_delete");
    tryExecute(query,
               "CREATE TRIGGER clean_orphaned_tag"
               " AFTER DELETE ON tasktag"
               " BEGIN DELETE FROM tag"
               " WHERE id = OLD.tag_id AND"
               "(SELECT count(*) FROM tasktag"
               " WHERE tag_id = OLD.tag_id) = 0;"
               "END;");
}

} // namespace sprint_timer::storage::qt_storage
