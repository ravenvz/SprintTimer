/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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

namespace InfoTable {
    const QString name{"info"};

    namespace Columns {
        const QString id{"id"};
        const QString name{"name"};
        const QString value{"value"};
    }
}

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
    }
}

namespace SprintTable {
    const QString name{"sprint"};

    namespace Columns {
        const QString id{"id"};
        const QString taskUuid{"task_uuid"};
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
    const QString name{"tasktag"};

    namespace Columns {
        const QString id{"id"};
        const QString tagId{"tag_id"};
        const QString taskId{"task_id"};
        const QString taskUuid{"task_uuid"};
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

namespace SprintView {
    const QString name{"sprint_view"};

    namespace Aliases {
        const QString tags{"tags"};
    }
}

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
 */
class Database {

public:
    Database(const QString& filename);

private:
    const QString filename;
    const QString connectionName{"test_connection"};

    bool prepare();

    bool create(QSqlDatabase& db);

    bool createSchema(QSqlDatabase& db);

    bool createTables(QSqlQuery& query);

    bool createViews(QSqlQuery& query);

    bool createTriggers(QSqlQuery& query);

    bool populateInfoTable(QSqlDatabase& database) const;

    bool testConnection(QSqlDatabase& database);

    bool upgradeIfRequired(QSqlDatabase& database);

    int getDatabaseVersion(QSqlDatabase& database);

    bool runMigration(QSqlDatabase& database, int fromVersion);

    bool createBackupCopy(const QString& filename);

    bool restoreBackupCopy(const QString& filename);

    bool updateVersion(QSqlDatabase& database) const;

    bool execAndCheck(QSqlQuery& query, const QString& queryStr);
};

#endif /* end of include guard: DORODATABASE_H_EY9GHBP5 */
