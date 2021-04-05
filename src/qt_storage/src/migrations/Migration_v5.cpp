/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "qt_storage/migrations/Migration_v5.h"
#include "qt_storage/utils/QueryUtils.h"

namespace sprint_timer::storage::qt_storage {

void Migration_v5::run(const QString& connectionName) const
{
    QSqlQuery query{QSqlDatabase::database(connectionName)};
    tryExecute(query, "DROP TRIGGER instead_sprint_view_insert");
    tryExecute(query, "DROP TRIGGER delete_from_sprint_view");
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

} // namespace sprint_timer::storage::qt_storage
