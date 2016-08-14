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

#include "qt_storage_impl/PomodoroDatabase.h"

#include <QDebug>
#include <QSqlError>


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

bool PomodoroDatabase::createSchema() {
    QSqlQuery query;

    const QString createTodoItemTable{
        "CREATE TABLE " + TaskTable::name + "("
            + TaskTable::Columns::id + " INTEGER PRIMARY KEY AUTOINCREMENT, "
        + TaskTable::Columns::name + " TEXT, "
        + TaskTable::Columns::estimatedPomodoros + " INTEGER, "
        + TaskTable::Columns::spentPomodoros + " INTEGER, "
        + TaskTable::Columns::completed + " BOOLEAN, "
        + TaskTable::Columns::priority + " INTEGER, "
        + TaskTable::Columns::lastModified + " DATETIME, "
        + TaskTable::Columns::uuid + " TEXT UNIQUE NOT NULL)"
    };

    const QString createPomodoroTable{
        "CREATE TABLE " + PomodoroTable::name + "("
        + PomodoroTable::Columns::id + " INTEGER PRIMARY KEY AUTOINCREMENT, "
        + PomodoroTable::Columns::taskUuid + " TEXT, "
        + PomodoroTable::Columns::startTime + " DATETIME, "
        + PomodoroTable::Columns::finishTime + " DATETIME, "
        + PomodoroTable::Columns::uuid + " TEXT UNIQUE NOT NULL, "
        + "FOREIGN KEY (" + PomodoroTable::Columns::taskUuid + ") "
        + "REFERENCES " + TaskTable::name + "(" + TaskTable::Columns::uuid + ")"
        + " ON DELETE CASCADE)"
    };

    const QString createTagTable{
        "CREATE TABLE " + TagTable::name
        + "(" + TagTable::Columns::id + " INTEGER PRIMARY KEY AUTOINCREMENT, "
        + TagTable::Columns::name + " TEXT UNIQUE NOT NULL)"
    };

    const QString createTodoTagTable{
        "CREATE TABLE " + TaskTagTable::name + "("
        + TaskTagTable::Columns::id + " INTEGER PRIMARY KEY AUTOINCREMENT, "
        + TaskTagTable::Columns::tagId + " INTEGER NOT NULL, "
        + TaskTagTable::Columns::taskId + " INTEGER NOT NULL, "
        + TaskTagTable::Columns::taskUuid + " TEXT NOT NULL, "
        + "FOREIGN KEY(" + TaskTagTable::Columns::tagId + ")"
        + " REFERENCES " + TagTable::name + "(" + TagTable::Columns::id + ")"
        + " ON DELETE CASCADE ON UPDATE CASCADE, "
        + "FOREIGN KEY(" + TaskTagTable::Columns::taskId + ")"
        + " REFERENCES " + TaskTable::name + "(" + TaskTable::Columns::id + ")"
        + " ON DELETE CASCADE ON UPDATE CASCADE)"
    };

    // This view is a workaround to let insertion of tags
    // when we don't know tag id of already existing tag
    QString createTaskTagView{
        "CREATE VIEW " + TaskTagView::name + " AS "
        + "SELECT " + TaskTable::name + "." + TaskTable::Columns::id + ", "
        + TaskTable::name + "." + TaskTable::Columns::name + ", "
        + TaskTable::Columns::estimatedPomodoros + ", "
        + TaskTable::Columns::spentPomodoros + ", "
        + TaskTable::Columns::priority + ", "
        + TaskTable::Columns::completed + ", "
        + TagTable::name + "." + TagTable::Columns::name + " " + TaskTagView::Aliases::tagName + ", "
        + TaskTable::Columns::lastModified + ", "
        + TaskTable::Columns::uuid
        + " FROM " + TaskTable::name
        + " JOIN " + TaskTagTable::name + " ON "
        + TaskTable::name + "." + TaskTable::Columns::id + " = " + TaskTagTable::name + "." + TaskTagTable::Columns::taskId
        + " LEFT JOIN " + TagTable::name + " ON "
        + TaskTagTable::name + "." + TaskTagTable::Columns::tagId + " = " + TagTable::name + "." + TagTable::Columns::id
        + " ORDER BY " + TaskTable::name + "." + TaskTable::Columns::priority + ";"
    };

    const QString createInsteadOfTaskTagInsertTrigger{
        "CREATE TRIGGER " + TaskTagInsertTrigger::name
        + " INSTEAD OF INSERT ON " + TaskTagView::name
        + " BEGIN INSERT INTO " + TagTable::name + "(" + TagTable::Columns::name +")"
        + " SELECT NEW." + TaskTagView::Aliases::tagName
        + " WHERE (SELECT COUNT(" + TagTable::Columns::name + ")"
        + " FROM " + TagTable::name
        + " WHERE " + TagTable::Columns::name + " = NEW." + TaskTagView::Aliases::tagName +") = 0; "
        + " INSERT OR IGNORE INTO "
        + TaskTagTable::name + "(" + TaskTagTable::Columns::tagId + ", " + TaskTagTable::Columns::taskId + ", " + TaskTagTable::Columns::taskUuid + ")"
        + " VALUES((SELECT " + TagTable::Columns::id +
        + " FROM " + TagTable::name
        + " WHERE " + TagTable::Columns::name + " = NEW." + TaskTagView::Aliases::tagName + "), "
        + "(SELECT " + TaskTable::Columns::id
        + " FROM " + TaskTable::name
        + " WHERE " + TaskTable::Columns::uuid + " = NEW." + TaskTable::Columns::uuid + "), NEW." + TaskTable::Columns::uuid + "); "
        + "END;"
    };

    // Trigger to remove orphaned tags (tags, that are not bound to any todo
    // item)
    const QString createCleanOrphanedTagTrigger{
        "CREATE TRIGGER " + CleanOrphanedTagTrigger::name
        + " AFTER DELETE ON " + TaskTagTable::name
        + " BEGIN "
        + "DELETE FROM " + TagTable::name
        + " WHERE " + TagTable::Columns::id + " = OLD." + TaskTagTable::Columns::tagId + " AND "
        + "(SELECT count(*) FROM " + TaskTagTable::name
        + " WHERE " + TagTable::Columns::id + " = OLD." + TaskTagTable::Columns::tagId + ") = 0; "
        + "END;"
    };

    // View for pomodoros
    const QString createPomodoroView{
        "CREATE VIEW " + PomoView::name + " AS "
        + "SELECT " + PomodoroTable::name + "." + PomodoroTable::Columns::id + ", "
        +  PomodoroTable::name + "." + PomodoroTable::Columns::taskUuid + ", "
        +  TaskTable::name + "." + TaskTable::Columns::name + ", "
        + "GROUP_CONCAT( " + TagTable::name + "." + TagTable::Columns::name + ") " + PomoView::Aliases::tags + ", "
        + PomodoroTable::Columns::startTime + ", "
        + PomodoroTable::Columns::finishTime + ", "
        + PomodoroTable::name + "." + PomodoroTable::Columns::uuid
        + " FROM " + PomodoroTable::name
        + " JOIN " + TaskTable::name + " ON "
        + PomodoroTable::name + "." + PomodoroTable::Columns::taskUuid + " = " + TaskTable::name + "." + TaskTable::Columns::uuid
        + " LEFT JOIN " + TaskTagTable::name + " ON " + TaskTagTable::name + "." + TaskTagTable::Columns::taskId + " = " + TaskTable::name + "." + TaskTable::Columns::id
        + " LEFT JOIN " + TagTable::name
        + " ON " + TagTable::name + "." + TagTable::Columns::id + " = " + TaskTagTable::name + "." + TaskTagTable::Columns::tagId
        + " GROUP BY " + PomodoroTable::name + "." + PomodoroTable::Columns::id + ";"
    };

    // Trigger to remove from pomodoro_view as view are read-only in Sqlite3
    const QString createPomodoroViewDeleteTrigger{
        "CREATE TRIGGER " + PomoViewDeleteTrigger::name
        + " INSTEAD OF DELETE ON " + PomoView::name
        + " BEGIN "
        + "DELETE FROM " + PomodoroTable::name
        + " WHERE " + PomodoroTable::Columns::uuid + " = OLD." + PomodoroTable::Columns::uuid + "; "
        + " END;"
    };

    const QString createPomodoroViewInsertTrigger{
        "CREATE TRIGGER " + PomoViewInsertTrigger::name
        + " INSTEAD OF INSERT ON " + PomoView::name
        + " BEGIN "
        + "INSERT INTO " + PomodoroTable::name + "("
        + PomodoroTable::Columns::taskUuid + ", "
        + PomodoroTable::Columns::startTime + ", "
        + PomodoroTable::Columns::finishTime + ", "
        + PomodoroTable::Columns::uuid + ") "
        + "SELECT"
        + " NEW." + PomodoroTable::Columns::taskUuid
        + ", NEW." + PomodoroTable::Columns::startTime
        + ", NEW." + PomodoroTable::Columns::finishTime
        + ", NEW." + PomodoroTable::Columns::uuid + "; "
        + "END;"
    };

    // View for todo items
    const QString createTaskView{
        "CREATE VIEW " + TasksView::name + " AS "
        + "SELECT " + TaskTable::name + "." + TaskTable::Columns::id + ", "
        + TaskTable::name + "." + TaskTable::Columns::name + ", "
        + TaskTable::Columns::estimatedPomodoros + ", "
        + TaskTable::Columns::spentPomodoros + ", "
        + TaskTable::Columns::priority + ", "
        + TaskTable::Columns::completed + ", "
        + "GROUP_CONCAT(" + TagTable::name + "." + TagTable::Columns::name + ") " +  TasksView::Aliases::tags + ", "
        + TaskTable::Columns::lastModified + ", "
        + TaskTable::Columns::uuid
        + " FROM " + TaskTable::name
        + " LEFT JOIN " + TaskTagTable::name + " ON "
        + TaskTable::name + "." + TaskTable::Columns::id + " = " + TaskTagTable::name + "." + TaskTagTable::Columns::taskId
        + " LEFT JOIN " + TagTable::name +  " ON "
        + TaskTagTable::name + "." + TaskTagTable::Columns::tagId + " = " + TagTable::name + "." + TagTable::Columns::id
        + " GROUP BY " + TaskTable::name + "." + TaskTable::Columns::id
        + " ORDER BY " + TaskTable::name + "." + TaskTable::Columns::priority + ";"
    };

    // Trigger to remove from task_view
    const QString createTaskViewDeleteTrigger{
        "CREATE TRIGGER " + TaskViewDeleteTrigger::name
        + " INSTEAD OF DELETE ON " + TasksView::name
        + " BEGIN "
        + "DELETE FROM " + TaskTable::name
        + " WHERE " + TaskTable::Columns::id + " = OLD." + TaskTable::Columns::id + ";"
        + " END;"
    };

    // Trigger on update on task_view
    const QString createTaskViewUpdateTrigger{
        "CREATE TRIGGER " + TaskViewUpdateTrigger::name
        + " INSTEAD OF UPDATE ON " + TasksView::name
        + " BEGIN "
        + "UPDATE " + TaskTable::name
        + " SET " + TaskTable::Columns::name + " = NEW." + TaskTable::Columns::name + ", "
        + TaskTable::Columns::estimatedPomodoros + " = NEW." + TaskTable::Columns::estimatedPomodoros + ", "
        + TaskTable::Columns::spentPomodoros + " = NEW." + TaskTable::Columns::spentPomodoros + ", "
        + TaskTable::Columns::priority + " = NEW." + TaskTable::Columns::priority + ", "
        + TaskTable::Columns::completed + " = NEW." + TaskTable::Columns::completed + ", "
        + TaskTable::Columns::lastModified + " = NEW." + TaskTable::Columns::lastModified
        + " WHERE " + TaskTable::Columns::id + " = OLD." + TaskTable::Columns::id + ";"
        + " END;"
    };

    // Create calendar table as temporary solution for missing dates problem
    query.exec("CREATE TABLE " + CalendarTable::name
            + " (" + CalendarTable::Columns::id + " INTEGER PRIMARY KEY)");
    query.exec("INSERT INTO " + CalendarTable::name + " DEFAULT VALUES");
    query.exec("INSERT INTO " + CalendarTable::name + " DEFAULT VALUES");
    query.exec("INSERT INTO " + CalendarTable::name + " SELECT NULL FROM "
            + CalendarTable::name + " d1, "
            + CalendarTable::name + " d2, "
            + CalendarTable::name + " d3, "
            + CalendarTable::name + " d4"
            );
    query.exec("INSERT INTO " + CalendarTable::name + " SELECT NULL FROM "
            + CalendarTable::name + " d1, "
            + CalendarTable::name + " d2, "
            + CalendarTable::name + " d3, "
            + CalendarTable::name + " d4 "
            );
    query.exec("ALTER TABLE " + CalendarTable::name + " ADD " + CalendarTable::Columns::dt + " DATETIME");
    query.exec(
        "UPDATE " + CalendarTable::name + " SET " + CalendarTable::Columns::dt
        + " = date('2000-01-01', (-1 + id) ||' day')");
    query.exec("CREATE INDEX idx1 ON " + CalendarTable::name + "(" + CalendarTable::Columns::dt + ")");

    qDebug() << createTaskView;

    return execAndCheck(query, createTodoItemTable)
        && execAndCheck(query, createTagTable)
        && execAndCheck(query, createPomodoroTable)
        && execAndCheck(query, createTodoTagTable)
        && execAndCheck(query, createTaskTagView)
        && execAndCheck(query, createInsteadOfTaskTagInsertTrigger)
        && execAndCheck(query, createCleanOrphanedTagTrigger)
        // && execAndCheck(query, createIncrementSpentTrigger)
        // && execAndCheck(query, createDecrementSpentTrigger)
        && execAndCheck(query, createPomodoroView)
        && execAndCheck(query, createPomodoroViewDeleteTrigger)
        && execAndCheck(query, createPomodoroViewInsertTrigger)
        && execAndCheck(query, createTaskView)
        && execAndCheck(query, createTaskViewDeleteTrigger)
        && execAndCheck(query, createTaskViewUpdateTrigger);
}

bool PomodoroDatabase::execAndCheck(QSqlQuery& query, const QString& queryStr) {
    bool ok = query.exec(queryStr);
    if (!ok) {
        qDebug() << "Error when running";
        qDebug() << queryStr;
        qDebug() << query.lastError();
    }
    return ok;
}

PomodoroDatabase::PomodoroDatabase(QString filename)
    : filename{std::move(filename)}
    , db{QSqlDatabase::addDatabase("QSQLITE")}
{

}

PomodoroDatabase::~PomodoroDatabase() {

}

bool PomodoroDatabase::create() {
    db.setDatabaseName(filename);
    return db.open() && createSchema();
}
