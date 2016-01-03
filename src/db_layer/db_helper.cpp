#include "db_helper.h"

void createDatabase(QSqlDatabase& db, QString& filename) {
    db.setDatabaseName(filename);
    db.open();
    QSqlQuery query;
    query.exec("pragma foreign_keys = on");
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
            "foreign key(tag_id) references tag(id) on delete cascade on update cascade, "
            "foreign key(todo_id) references todo_item(id) on delete cascade on update cascade)");
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
    return true;
}
