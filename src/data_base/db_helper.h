#ifndef DB_HELPER_H
#define DB_HELPER_H

#include <QtSql>
#include <QSqlDatabase>
#include "core/entities.h"


void createDatabase(QSqlDatabase& db, QString& filename);

bool createDbConnection();


class PomodoroGateway
{
public:
    static QStringList getPomodorosForToday() {
        QStringList result;
        QSqlQuery query;
        // TODO replace star
        query.exec("select * from pomodoro where date(start_time) = date('now') order by start_time desc");
        while (query.next()) {
            QStringList m;
            Pomodoro pomodoro {query.value(1).toString(),
                               query.value(2).toDateTime(),
                               query.value(3).toDateTime()};
            result.append(pomodoro.asString());
        }
        return result;
    }

    static void storePomodoro(Pomodoro pomodoro) {
        QSqlQuery query;
        query.prepare("insert into pomodoro (name, start_time, finish_time) "
                      "values (:name, :start_time, :finish_time)");
        query.bindValue(":name", QVariant(pomodoro.name));
        query.bindValue(":start_time", QVariant(pomodoro.startTime));
        query.bindValue(":finish_time", QVariant(pomodoro.finishTime));
        query.exec();
    }
};


class TodoItemGateway
{
public:
    static void storeTodoItem(TodoItem item) {
        QSqlQuery query;
        query.prepare("insert into todo_item (name, estimated_pomodoros, spent_pomodoros, completed, priority, last_modified) "
                "values (:name, :estimated_pomodoros, :spent_pomodoros, :completed, :priority, :last_modified)");
        query.bindValue(":name", QVariant(item.name));
        query.bindValue(":estimated_pomodoros", QVariant(item.estimatedPomodoros));
        query.bindValue(":spent_pomodoros", QVariant(item.spentPomodoros));
        query.bindValue(":completed", QVariant(item.completed));
        query.bindValue(":priority", QVariant(item.priority));
        query.bindValue(":last_modified", QVariant(QDateTime::currentDateTime()));
        query.exec();
        QVariant todoId = query.lastInsertId();
        // std::cout << todoId.toInt() << std::endl;
        qDebug() << todoId.toInt();
        if (!item.tags.isEmpty()) {
            for (QString tag : item.tags) {
                query.prepare("select id from tag where name = (:name)");
                query.bindValue(":name", QVariant(tag));
                query.exec();
                QVariant tagId;
                if (query.next()) {
                    qDebug() << "Found tag";
                    tagId = query.value(0);
                } else {
                    qDebug() << "NOT fount tag";
                    query.prepare("insert into tag (name) values (:name)");
                    query.bindValue(":name", QVariant(tag));
                    query.exec();
                    tagId = query.lastInsertId();
                }

                query.prepare("insert into todotag (tag_id, todo_id) values (:tag_id, :todo_id)");
                query.bindValue(":tag_id", tagId);
                query.bindValue(":todo_id", todoId);
                query.exec();
            }
        }
    }

    static QStringList getUncompleteTodoItems() {
        QSqlQuery query;
        query.exec("select todo_item.name, todo_item.estimated_pomodoros, "
               "todo_item.spent_pomodoros, todo_item.priority, todo_item.completed, group_concat(tag.name) "
               "from todo_item join todotag on todo_item.id = todotag.todo_id "
               "join tag on tag.id = todotag.tag_id "
               "where completed = 0 "
               "group by todo_item.name");
        QStringList result;
        while (query.next()) {
            QStringList tags = query.value(5).toString().split(",");
            TodoItem item {query.value(0).toString(),
                           query.value(1).toUInt(),
                           query.value(2).toUInt(),
                           query.value(3).toUInt(),
                           tags,
                           query.value(4).toBool()};
            result.append(item.asString());
        }
        return result;
    }
};

#endif // DB_HELPER_H
