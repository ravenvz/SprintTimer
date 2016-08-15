/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/

#ifndef POMODORODATABASE_H_EY9GHBP5
#define POMODORODATABASE_H_EY9GHBP5


#include <QSqlQuery>
#include <QString>


namespace TaskTable {
    const QString name{"todo_item"};

    namespace Columns {
        const QString id{"id"};
        const QString name{"name"};
        const QString estimatedPomodoros{"estimated_pomodoros"};
        const QString spentPomodoros{"spent_pomodoros"};
        const QString completed{"completed"};
        const QString priority{"priority"};
        const QString lastModified{"last_modified"};
        const QString uuid{"uuid"};
    }
}

namespace PomodoroTable {
    const QString name{"pomodoro"};

    namespace Columns {
        const QString id{"id"};
        const QString taskUuid{"todo_uuid"};
        const QString startTime{"start_time"};
        const QString finishTime{"finish_time"};
        const QString uuid{"uuid"};
    }
}

namespace TagTable {
    const QString name{"tag"};

    namespace Columns {
        const QString id{"id"};
        const QString name{"name"};
    }
}

namespace TaskTagTable {
    const QString name{"todotag"};

    namespace Columns {
        const QString id{"id"};
        const QString tagId{"tag_id"};
        const QString taskId{"todo_id"};
        const QString taskUuid{"todo_uuid"};
    }
}

namespace TaskTagView
{
    const QString name{"task_tag_view"};

    namespace Aliases {
        const QString tagName{"tagname"};
    }
} /* TaskTagView */

namespace TaskTagInsertTrigger {
    const QString name{"instead_task_tag_view_insert"};
}

namespace CleanOrphanedTagTrigger {
    const QString name{"on_todo_tag_delete"};
}

namespace PomoView {
    const QString name{"pomodoro_view"};

    namespace Aliases {
        const QString tags{"tags"};
    }
}

namespace PomoViewDeleteTrigger {
    const QString name{"delete_from_pomodoro_view"};
}

namespace PomoViewInsertTrigger {
    const QString name{"instead_pomodoro_view_insert"};
}

namespace TasksView {
    const QString name{"task_view"};

    namespace Aliases {
        const QString tags{"tags"};
    }
}

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
    }
}


/* Responsible for creating database and schema.
 * If database already exists, tries to establish connection and exits on success
 * and throws std::runtime_error when database could not be found and(or) created.
 *
 * TODO track database version to handle migrations
 */
class PomodoroDatabase {

public:
    PomodoroDatabase(const QString& filename);

private:
    const QString filename;
    const QString connectionName{"test_connection"};

    bool prepare();

    bool create(QSqlDatabase& db);

    bool createSchema(QSqlDatabase& db);

    bool execAndCheck(QSqlQuery& query, const QString& queryStr);
};



#endif /* end of include guard: POMODORODATABASE_H_EY9GHBP5 */
