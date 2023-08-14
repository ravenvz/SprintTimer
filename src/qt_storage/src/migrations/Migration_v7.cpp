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
#include "qt_storage/migrations/Migration_v7.h"
#include "qt_storage/TransactionGuard.h"
#include "qt_storage/utils/QueryUtils.h"

namespace sprint_timer::storage::qt_storage {

Migration_v7::Migration_v7(api::UUIDGenerator& uuidGenerator_)
    : uuidGenerator{uuidGenerator_}
{
}

auto Migration_v7::run(const QString& connectionName) const -> void
{
    QSqlQuery query{QSqlDatabase::database(connectionName)};

    const auto inboxFolderUuid =
        QString::fromStdString(uuidGenerator.generateUUID());

    tryExecute(query, "DROP VIEW sprint_view;");
    tryExecute(query, "DROP VIEW task_view;");
    tryExecute(query, "DROP VIEW task_tag_view;");
    tryExecute(query, "DROP TRIGGER clean_orhaned_tag;");

    tryExecute(query,
               "ALTER TABLE task "
               "ADD COLUMN deleted BOOLEAN DEFAULT 0;");

    tryExecute(query,
               "ALTER TABLE task "
               "ADD COLUMN type INTEGER DEFAULT 2;");

    // tryExecute(query,
    //            "CREATE TABLE IF NOT EXISTS task_temp ("
    //            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    //            "uuid TEXT UNIQUE NOT NULL, "
    //            "name TEXT, "
    //            "estimated_cost INTEGER, "
    //            "completed BOOLEAN, "
    //            "last_modified DATETIME, "
    //            "deleted BOOLEAN DEFAULT 0);");

    // tryExecute(
    //     query,
    //     "INSERT INTO task_temp(id, uuid, name, estimated_cost, completed, "
    //     "last_modified, deleted, )"
    //     "SELECT id, uuid, name, estimated_cost, completed, last_modified, "
    //     "deleted "
    //     "FROM task; ");
    //
    // tryExecute(query, "DROP TABLE task;");
    // tryExecute(query, "ALTER TABLE task_temp RENAME TO task;");

    tryExecute(query,
               "CREATE TABLE IF NOT EXISTS note ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "task_id INTEGER UNIQUE NOT NULL, "
               "text STRING, "
               "FOREIGN KEY (task_id) REFERENCES task(id) ON DELETE CASCADE);");

    tryExecute(query,
               "CREATE TABLE IF NOT EXISTS task_timeframe ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "task_id INTEGER UNIQUE NOT NULL, "
               "start DATETIME DEFAULT NULL, "
               "due DATETIME DEFAULT NULL, "
               "reminder DATETIME DEFAULT NULL, "
               "recurrence TEXT DEFAULT NULL, "
               "FOREIGN KEY (task_id) REFERENCES task(id) ON DELETE CASCADE);");

    tryExecute(query,
               "ALTER TABLE sprint "
               "ADD COLUMN deleted BOOLEAN DEFAULT 0; ");

    tryExecute(query,
               "CREATE TABLE IF NOT EXISTS sprint_temp ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT, "
               "task_id INTEGER, "
               "start_time DATETIME, "
               "finish_time DATETIME, "
               "deleted BOOLEAN DEFAULT 0, "
               "FOREIGN KEY (task_id) REFERENCES task(id) ON DELETE CASCADE);");

    tryExecute(
        query,
        "INSERT INTO sprint_temp(id, task_id, start_time, finish_time, "
        "deleted) "
        "SELECT id, (SELECT id FROM task WHERE uuid = sprint.task_uuid), "
        "start_time, finish_time, deleted "
        "FROM sprint;");

    tryExecute(query, "DROP TABLE sprint;");

    tryExecute(query, "ALTER TABLE sprint_temp RENAME TO sprint;");

    tryExecute(query,
               "CREATE VIEW sprint_view AS "
               "SELECT task.id taskid, task.name, GROUP_CONCAT(tag.name) tags, "
               "start_time, finish_time "
               "FROM sprint "
               "JOIN task ON "
               "sprint.task_id = task.id "
               "LEFT JOIN tasktag "
               "ON task.id = tasktag.task_id "
               "LEFT JOIN tag ON tasktag.tag_id = tag.id "
               "WHERE sprint.deleted = 0 "
               "GROUP BY sprint.id;");

    tryExecute(query,
               "CREATE VIEW clean_sprint_view AS "
               "SELECT id, task_id, start_time, finish_time "
               "FROM sprint WHERE deleted = 0;");

    tryExecute(query,
               "CREATE VIEW task_tag_view AS "
               "SELECT task.id id, task.name name, estimated_cost, completed, "
               "tag.name tagname, last_modified, uuid, type "
               "FROM task "
               "JOIN tasktag on task.id = tasktag.task_id "
               "LEFT JOIN tag ON tag.id = tasktag.tag_id "
               "WHERE deleted = 0;");

    tryExecute(query,
               "CREATE VIEW task_view AS "
               "SELECT task.id task_id, task.name name, estimated_cost, "
               "completed, "
               "GROUP_CONCAT(tag.name) tags, last_modified, uuid, type, "
               "text, start, due, reminder, recurrence "
               "FROM task "
               "LEFT JOIN tasktag ON task.id = tasktag.task_id "
               "LEFT JOIN tag ON tasktag.tag_id = tag.id "
               "LEFT JOIN note ON task.id = note.task_id "
               "LEFT JOIN task_timeframe ON task.id = task_timeframe.task_id "
               "WHERE deleted = 0 "
               "GROUP BY task.id;");

    tryExecute(query,
               "CREATE VIEW adv_task_view AS "
               "SELECT uuid, name, estimated_cost, completed, "
               "tags, last_modified, start_time, finish_time, "
               "text, start, due, reminder, recurrence, type, "
               "task_view.task_id id "
               "FROM task_view "
               "LEFT JOIN clean_sprint_view "
               "ON clean_sprint_view.task_id = task_view.task_id;");

    tryExecute(query,
               "CREATE TABLE IF NOT EXISTS task_tree ("
               "task_uuid STRING);");

    // Writing serialized version of task tree into the table placing all tasks
    // that are considered active (see query details for definition) into newly
    // created folder Inbox
    tryExecute(query, "INSERT INTO task_tree(task_uuid) VALUES(NULL);");
    tryExecute(query, "INSERT INTO task_tree(task_uuid) VALUES(NULL);");
    tryExecute(query,
               QString{"INSERT INTO task_tree(task_uuid) VALUES('%1');"}.arg(
                   inboxFolderUuid));
    tryExecute(query, "INSERT INTO task_tree(task_uuid) VALUES(NULL);");
    tryExecute(query,
               "INSERT INTO task_tree(task_uuid) SELECT uuid FROM task_view "
               "WHERE completed = 0 OR last_modified > "
               "DATETIME('now', '-1 day');");
    tryExecute(
        query,
        "WITH RECURSIVE generate_series(value) AS (SELECT 1 UNION ALL SELECT "
        "value + 1 FROM generate_series WHERE value + 1 <= (SELECT count(*) + "
        "1 "
        "FROM task_view WHERE completed = 0 OR "
        "last_modified > DATETIME('now', '-1 day'))) INSERT INTO "
        "task_tree(task_uuid) SELECT NULL FROM "
        "generate_series;");
    tryExecute(
        query,
        QString{"INSERT INTO task(name, estimated_cost, actual_cost, "
                " completed, "
                "priority, last_modified, uuid, deleted, type) "
                " values('Inbox', 0, 0, 0, 0, DATETIME('now'), '%1', 0, 1);"}
            .arg(inboxFolderUuid));

    // "LEFT JOIN note ON note.task_id = task_view.task_id;");

    // "CREATE VIEW adv_task_view AS "
    // "SELECT uuid, task.name task_name, estimated_cost, completed, "
    // "GROUP_CONCAT(tag.name) tags, last_modified, start_time, finish_time
    // " "FROM task " "LEFT JOIN tasktag ON tasktag.task_id = task.id "
    // "LEFT JOIN tag ON tasktag.tag_id = tag.id "
    // "LEFT JOIN sprint "
    // "ON task.id = sprint.task_id "
    // "WHERE sprint.deleted = 0 AND task.deleted = 0 "
    // "GROUP BY task.id;"

    tryExecute(
        query,
        "CREATE TRIGGER instead_task_tag_view_insert "
        "INSTEAD OF INSERT ON task_tag_view "
        "BEGIN INSERT INTO tag(name) "
        "SELECT NEW.tagname "
        "WHERE (SELECT COUNT(name) FROM tag WHERE name = NEW.tagname) = 0; "
        "INSERT OR IGNORE INTO tasktag(tag_id, task_id, task_uuid) "
        "VALUES ((SELECT id FROM tag WHERE name = NEW.tagname), (SELECT id "
        "FROM task WHERE uuid = NEW.uuid), NEW.uuid); END;");

    // tryExecute(
    //     query,
    //     "CREATE TRIGGER instead_task_tag_view_insert "
    //     "INSTEAD OF INSERT ON task_tag_view "
    //     "BEGIN INSERT INTO tag(name) "
    //     "SELECT NEW.tagname WHERE "
    //     "(SELECT COUNT(name) FROM tag WHERE name = NEW.tagname) = 0;  "
    //     "INSERT OR IGNORE INTO tasktag(tag_id, task_id, task_uuid) "
    //     "VALUES((SELECT id FROM tag WHERE name = NEW.tagname), "
    //     "(SELECT id FROM task WHERE uuid = NEW.uuid), NEW.uuid); END;");

    tryExecute(query,
               "CREATE TRIGGER clean_orhaned_tag "
               "AFTER DELETE ON tasktag "
               "BEGIN DELETE FROM tag "
               "WHERE id = OLD.tag_id AND "
               "(SELECT COUNT(*) FROM tasktag WHERE tag_id = OLD.tag_id) = 0;"
               "END;");

    tryExecute(query,
               "CREATE TRIGGER on_task_view_delete "
               "INSTEAD OF DELETE ON task_view BEGIN "
               "DELETE FROM task "
               "WHERE id = OLD.id; END;");

    tryExecute(
        query,
        "CREATE TRIGGER on_adv_task_view_update "
        "INSTEAD OF UPDATE ON adv_task_view "
        "BEGIN "
        "UPDATE task SET name = NEW.name, estimated_cost = NEW.estimated_cost, "
        "completed = NEW.completed, last_modified = NEW.last_modified, type = "
        "NEW.type "
        "WHERE id = OLD.id; "
        "INSERT INTO note(task_id, text) "
        "VALUES (OLD.id, NEW.text) "
        "ON CONFLICT(task_id) DO UPDATE SET text=NEW.text; "
        "UPDATE task_timeframe "
        "SET start = NEW.start, due = NEW.due, reminder = NEW.reminder, "
        "recurrence = NEW.recurrence "
        "WHERE task_id = OLD.id;"
        "END;");

    tryExecute(query,
               "CREATE TRIGGER clean_empty_notes "
               "AFTER UPDATE ON note "
               "BEGIN "
               "DELETE FROM note WHERE text IS NULL; "
               "END;");

    // tryExecute(query,
    //            "CREATE TRIGGER on_task_view_update "
    //            "INSTEAD OF UPDATE ON task_view BEGIN "
    //            "UPDATE task SET name = NEW.name, estimated_cost = "
    //            "NEW.estimated_cost, completed = NEW.completed, "
    //            "last_modified = NEW.last_modified, "
    //            "deleted = OLD.deleted "
    //            "WHERE id = OLD.id; END;");

    tryExecute(query,
               "CREATE TRIGGER delete_from_sprint_view "
               "INSTEAD OF DELETE ON sprint_view "
               "BEGIN DELETE FROM sprint "
               "WHERE start_time = OLD.start_time; END;");

    tryExecute(query,
               "CREATE TRIGGER instead_sprint_view_insert "
               "INSTEAD OF INSERT ON sprint_view "
               "BEGIN INSERT INTO sprint(task_id, start_time, finish_time) "
               "SELECT NEW.taskid, NEW.start_time, NEW.finish_time; END;");
}

} // namespace sprint_timer::storage::qt_storage
