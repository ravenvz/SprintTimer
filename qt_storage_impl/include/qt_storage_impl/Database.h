/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

#ifndef DATABASE_H_EY9GHBP5
#define DATABASE_H_EY9GHBP5

#include <QSqlQuery>
#include <QString>

namespace sprint_timer::storage::qt_storage_impl {

namespace InfoTable {
    const QString name{"info"};

    namespace Columns {
        const QString id{"id"};
        const QString name{"name"};
        const QString value{"value"};
    } // namespace Columns
} // namespace InfoTable

namespace TaskTable {
    const QString name{"task"};

    namespace Columns {
        const QString id{"id"};
        const QString name{"name"};
        const QString estimatedCost{"estimated_cost"};
        const QString actualCost{"actual_cost"};
        const QString completed{"completed"};
        const QString priority{"priority"};
        const QString lastModified{"last_modified"};
        const QString uuid{"uuid"};
    } // namespace Columns
} // namespace TaskTable

namespace SprintTable {
    const QString name{"sprint"};

    namespace Columns {
        const QString id{"id"};
        const QString taskUuid{"task_uuid"};
        const QString startTime{"start_time"};
        const QString finishTime{"finish_time"};
        const QString uuid{"uuid"};
    } // namespace Columns
} // namespace SprintTable

namespace TagTable {
    const QString name{"tag"};

    namespace Columns {
        const QString id{"id"};
        const QString name{"name"};
    } // namespace Columns
} // namespace TagTable

namespace TaskTagTable {
    const QString name{"tasktag"};

    namespace Columns {
        const QString id{"id"};
        const QString tagId{"tag_id"};
        const QString taskId{"task_id"};
        const QString taskUuid{"task_uuid"};
    } // namespace Columns
} // namespace TaskTagTable

namespace TaskTagView {
    const QString name{"task_tag_view"};

    namespace Aliases {
        const QString tagName{"tagname"};
    }
} // namespace TaskTagView

namespace TaskTagInsertTrigger {
    const QString name{"instead_task_tag_view_insert"};
}

namespace CleanOrphanedTagTrigger {
    const QString name{"clean_orhaned_tag"};
}

namespace SprintView {
    const QString name{"sprint_view"};

    namespace Aliases {
        const QString tags{"tags"};
    }
} // namespace SprintView

namespace SprintViewDeleteTrigger {
    const QString name{"delete_from_sprint_view"};
}

namespace SprintViewInsertTrigger {
    const QString name{"instead_sprint_view_insert"};
}

namespace TasksView {
    const QString name{"task_view"};

    namespace Aliases {
        const QString tags{"tags"};
    }
} // namespace TasksView

namespace TaskViewDeleteTrigger {
    const QString name{"on_task_view_delete"};
}

namespace TaskViewUpdateTrigger {
    const QString name{"on_task_view_update"};
}

namespace CalendarTable {
    const QString name{"calendar"};

    namespace Columns {
        const QString id{"id"};
        const QString dt{"dt"};
    } // namespace Columns
} // namespace CalendarTable


/* Responsible for creating database and schema.
 * If database already exists, tries to establish connection and exits on
 * success and throws std::runtime_error when database could not be found
 * and(or) created.
 */
class Database {

public:
    explicit Database(const QString& filename);

private:
    const QString filename;
    const QString connectionName{"test_connection"};
};

} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: DORODATABASE_H_EY9GHBP5 */
