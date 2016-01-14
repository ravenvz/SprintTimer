#include "db_helper.h"

void createSchema() {
    QSqlQuery query;
    query.exec("create table pomodoro "
            "(id integer primary key autoincrement, "
            "name varchar, "
            "todo_id integer, "
            "start_time datetime, "
            "finish_time datetime), "
            "foreign key (todo_id) references todo_item(id) on delete cascade on update cascade");
    query.exec("create table tag "
            "(id integer primary key autoincrement, "
            "name varchar unique not null)");
    query.exec("create table todo_item "
            "(id integer primary key autoincrement, "
            "name varchar, "
            "estimated_pomodoros integer, "
            "spent_pomodoros integer, "
            "completed boolean, "
            "priority integer, "
            "last_modified datetime)");
    query.exec("create table todotag "
            "(id integer primary key autoincrement, "
            "tag_id integer not null, "
            "todo_id integer not null, "
            "foreign key(tag_id) references tag(id), "
            "foreign key(todo_id) references todo_item(id) on delete cascade)");
    // Trigger to remove orphaned tags (tags, that are not bound to any todo item)
    query.exec("CREATE TRIGGER on_todotag_delete AFTER DELETE ON todotag "
               "BEGIN "
               "DELETE FROM tag WHERE id = old.tag_id AND "
               "(SELECT count(*) FROM todotag WHERE tag_id = old.tag_id) = 0;"
               "END;");
    query.exec("create trigger increment_spent_after_pomo_insert "
               "after insert on pomodoro "
               "begin "
               "update todo_items set spent_pomodoros = spent_pomodoros + 1 "
               "where todo_item.id = NEW.todo_id; "
               "end;");
    query.exec("create trigger decrement_spent_after_pomo_delete "
               "after delete on pomodoro "
               "begin "
               "update todo_items set spent_pomodoros = spent_pomodoros - 1 "
               "where todo_item.id = OLD.todo_id; "
               "end;");
    query.exec("create table calendar (id integer primary key)");
    query.exec("insert into calendar default values");
    query.exec("insert into calendar default values");
    query.exec("insert into calendar select null from calendar d1, calendar d2, calendar d3 , calendar d4");
    query.exec("insert into calendar select null from calendar d1, calendar d2, calendar d3 , calendar d4");
    query.exec("alter table calendar add dt datetime");
    query.exec("update calendar set dt=date('2000-01-01',(-1+id)||' day')");
    query.exec("CREATE INDEX idx1 on calendar(dt)");
    // View for pomodoros
    query.exec("CREATE VIEW pomodoro_view AS "
               "SELECT pomodoro.id, pomodoro.todo_id, todo_item.name, group_concat(tag.name) tags, start_time, finish_time "
               "FROM pomodoro join todo_item on pomodoro.todo_id = todo_item.id "
               "LEFT JOIN todotag ON todotag.todo_id = todo_item.id "
               "LEFT JOIN tag ON tag.id = todotag.tag_id "
               "GROUP BY pomodoro.id; ");
    // Trigger to remove from pomodoro_view as view are read-only in Sqlite3
    query.exec("CREATE TRIGGER delete_from_pomodoro_view INSTEAD OF DELETE ON pomodoro_view "
               "BEGIN "
               "DELETE FROM pomodoro WHERE id = old.id; "
               "END;");

    // View for todo items
    query.exec("CREATE VIEW task_view AS "
               "select todo_item.id, "
               "todo_item.name, "
               "estimated_pomodoros, "
               "spent_pomodoros, "
               "priority, "
               "completed, "
               "group_concat(tag.name) tags, "
               "last_modified "
               "from todo_item "
               "LEFT JOIN todotag on todo_item.id = todotag.todo_id "
               "LEFT JOIN tag ON todotag.tag_id = tag.id "
               "GROUP BY todo_item.id "
               "ORDER BY todo_item.priority;");
    // Trigger to remove from task_view
    query.exec("CREATE TRIGGER on_task_view_delete INSTEAD OF DELETE ON task_view "
               "BEGIN "
               "DELETE FROM todo_item WHERE id = old.id; "
               "END; ");
    // Trigger on update on task_view
    query.exec("CREATE TRIGGER on_task_view_update INSTEAD OF UPDATE ON task_view "
               "BEGIN "
               "UPDATE todo_item SET name = new.name, "
                         "estimated_pomodoros = new.estimated_pomodoros, "
                         "spent_pomodoros = new.spent_pomodoros, "
                         "priority = new.priority, "
                         "completed = new.completed, "
                         "last_modified = new.last_modified "
               "WHERE id = old.id; "
               "END;");
}

void createDatabase(QSqlDatabase& db, QString& filename) {
    db.setDatabaseName(filename);
    db.open();
    createSchema();
    // qDebug() << query.lastError() << " this error";
    // db.close();
}

bool createDbConnection() {
    QString filename = "db/pomodoro.db";
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QFile databaseFile(filename);
    if (!databaseFile.exists()) {
        qDebug() << "Database not found. Creating...";
        createDatabase(db, filename);
    } else {
        qDebug() << "Database found. Opening...";
        db.setDatabaseName(filename);
    }
    if (!db.open()) {
        return false;
    }
    activateForeignKeys();
    return true;
}

bool activateForeignKeys() {
    QSqlQuery query;
    return query.exec("PRAGMA foreign_keys = ON");
}
