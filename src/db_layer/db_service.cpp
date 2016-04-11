#include "db_service.h"
#include <exception>


DBService::DBService(QString filename)
    : filename{std::move(filename)}
    , db{QSqlDatabase::addDatabase("QSQLITE")}
{
    if (!createDbConnection())
        throw std::runtime_error("Unable to create database");
}


DBService::~DBService()
{
    db.close();
    QSqlDatabase::removeDatabase("QSQLITE");
}


bool DBService::createDatabase()
{
    db.setDatabaseName(filename);
    return db.open() && createSchema();
}


bool DBService::createSchema()
{
    QSqlQuery query;

    QString createTodoItemTable = "CREATE TABLE todo_item "
                                  "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                  "name VARCHAR, "
                                  "estimated_pomodoros INTEGER, "
                                  "spent_pomodoros INTEGER, "
                                  "completed BOOLEAN, "
                                  "priority INTEGER, "
                                  "last_modified DATETIME)";

    QString createPomodoroTable
        = "CREATE TABLE pomodoro "
          "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "todo_id INTEGER, "
          "start_time DATETIME, "
          "finish_time DATETIME, "
          "FOREIGN KEY (todo_id) REFERENCES todo_item(id) "
          "ON DELETE CASCADE)";

    QString createTagTable = "CREATE TABLE tag "
                             "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
                             "name VARCHAR(15) UNIQUE NOT NULL)";

    QString createTodoTagTable
        = "CREATE TABLE todotag "
          "(id INTEGER PRIMARY KEY AUTOINCREMENT, "
          "tag_id INTEGER NOT NULL, "
          "todo_id INTEGER NOT NULL, "
          "FOREIGN KEY(tag_id) REFERENCES tag(id), "
          "FOREIGN KEY(todo_id) REFERENCES todo_item(id) ON DELETE CASCADE)";

    // Trigger to remove orphaned tags (tags, that are not bound to any todo
    // item)
    QString createCleanOrphanedTagTrigger
        = "CREATE TRIGGER on_todotag_delete "
          "AFTER DELETE ON todotag "
          "BEGIN "
          "DELETE FROM tag WHERE id = old.tag_id AND "
          "(SELECT count(*) FROM todotag "
          "WHERE tag_id = old.tag_id) = 0;"
          "END;";

    // Trigger to increment spent pomodoros in todo_item when new pomodoro is
    // inserted
    QString createIncrementSpentTrigger
        = "CREATE TRIGGER increment_spent_after_pomo_insert "
          "AFTER INSERT ON pomodoro "
          "BEGIN "
          "UPDATE todo_item SET spent_pomodoros = spent_pomodoros + 1 "
          "WHERE todo_item.id = NEW.todo_id; "
          "END;";

    // Trigger to decrement spent pomodoros in todo_item after pomodoro is
    // removed
    QString createDecrementSpentTrigger
        = "CREATE TRIGGER decrement_spent_after_pomo_delete "
          "AFTER DELETE ON pomodoro "
          "BEGIN "
          "UPDATE todo_item SET spent_pomodoros = spent_pomodoros - 1 "
          "WHERE todo_item.id = OLD.todo_id; "
          "END;";

    // View for pomodoros
    QString createPomodoroView
        = "CREATE VIEW pomodoro_view AS "
          "SELECT pomodoro.id, pomodoro.todo_id, todo_item.name, "
          "group_concat(tag.name) tags, start_time, finish_time "
          "FROM pomodoro join todo_item ON pomodoro.todo_id = todo_item.id "
          "LEFT JOIN todotag ON todotag.todo_id = todo_item.id "
          "LEFT JOIN tag ON tag.id = todotag.tag_id "
          "GROUP BY pomodoro.id;";

    // Trigger to remove from pomodoro_view as view are read-only in Sqlite3
    QString createPomodoroViewDeleteTrigger
        = "CREATE TRIGGER delete_from_pomodoro_view "
          "INSTEAD OF DELETE ON pomodoro_view "
          "BEGIN "
          "DELETE FROM pomodoro WHERE id = old.id; "
          "END;";

    QString createPomodoroViewInsertTrigger
        = "CREATE TRIGGER instead_pomodoro_view_insert "
          "INSTEAD OF INSERT "
          "ON pomodoro_view "
          "BEGIN "
          "INSERT INTO pomodoro (todo_id, start_time, finish_time) "
          "SELECT new.todo_id, new.start_time, new.finish_time;"
          "END;";

    // View for todo items
    QString createTaskView
        = "CREATE VIEW task_view AS "
          "SELECT todo_item.id, "
          "todo_item.name, "
          "estimated_pomodoros, "
          "spent_pomodoros, "
          "priority, "
          "completed, "
          "GROUP_CONCAT(tag.name) tags, "
          "last_modified "
          "FROM todo_item "
          "LEFT JOIN todotag ON todo_item.id = todotag.todo_id "
          "LEFT JOIN tag ON todotag.tag_id = tag.id "
          "GROUP BY todo_item.id "
          "ORDER BY todo_item.priority;";

    // Trigger to remove from task_view
    QString createTaskViewDeleteTrigger
        = "CREATE TRIGGER on_task_view_delete "
          "INSTEAD OF DELETE ON task_view "
          "BEGIN "
          "DELETE FROM todo_item WHERE id = old.id; "
          "END;";

    // Trigger on update on task_view
    QString createTaskViewUpdateTrigger
        = "CREATE TRIGGER on_task_view_update "
          "INSTEAD OF UPDATE ON task_view "
          "BEGIN "
          "UPDATE todo_item SET name = new.name, "
          "estimated_pomodoros = new.estimated_pomodoros, "
          "spent_pomodoros = new.spent_pomodoros, "
          "priority = new.priority, "
          "completed = new.completed, "
          "last_modified = new.last_modified "
          "WHERE id = old.id; "
          "END;";

    // Create calendar table
    query.exec("CREATE TABLE calendar (id INTEGER PRIMARY KEY)");
    query.exec("INSERT INTO calendar DEFAULT VALUES");
    query.exec("INSERT INTO calendar DEFAULT VALUES");
    query.exec("INSERT INTO calendar SELECT NULL FROM calendar d1, calendar "
               "d2, calendar d3, calendar d4");
    query.exec("INSERT INTO calendar SELECT NULL FROM calendar d1, calendar "
               "d2, calendar d3, calendar d4");
    query.exec("ALTER TABLE calendar ADD dt DATETIME");
    query.exec(
        "UPDATE calendar SET dt = date('2000-01-01', (-1 + id) ||' day')");
    query.exec("CREATE INDEX idx1 ON calendar(dt)");

    return execAndCheck(query, createTodoItemTable)
        && execAndCheck(query, createTagTable)
        && execAndCheck(query, createPomodoroTable)
        && execAndCheck(query, createTodoTagTable)
        && execAndCheck(query, createCleanOrphanedTagTrigger)
        && execAndCheck(query, createIncrementSpentTrigger)
        && execAndCheck(query, createDecrementSpentTrigger)
        && execAndCheck(query, createPomodoroView)
        && execAndCheck(query, createPomodoroViewDeleteTrigger)
        && execAndCheck(query, createPomodoroViewInsertTrigger)
        && execAndCheck(query, createTaskView)
        && execAndCheck(query, createTaskViewDeleteTrigger)
        && execAndCheck(query, createTaskViewUpdateTrigger);
}


bool DBService::createDbConnection()
{
    // QString filename = "db/pomodoro.db";
    // QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QFile databaseFile(filename);
    if (filename == ":memory:") {
        db.open();
        createSchema();
        activateForeignKeys();
        return true;
    }
    // if (filename == ":memory:" || !databaseFile.exists()) {
    if (!databaseFile.exists()) {
        qDebug() << "Database not found. Creating...";
        if (!createDatabase()) {
            return false;
        }
    }
    else {
        qDebug() << "Database found. Opening...";
        db.setDatabaseName(filename);
    }

    if (!db.open()) {
        qDebug() << "Failed to open database";
        return false;
    }
    activateForeignKeys();
    return true;
}


bool DBService::activateForeignKeys()
{
    QSqlQuery query;
    return query.exec("PRAGMA foreign_keys = ON");
}


bool DBService::execAndCheck(QSqlQuery& query, const QString& queryStr)
{
    bool ok = query.exec(queryStr);
    if (!ok) {
        qDebug() << "Error when running";
        qDebug() << queryStr;
        qDebug() << query.lastError();
    }
    return ok;
}
