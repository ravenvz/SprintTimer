#ifndef DB_HELPER_H
#define DB_HELPER_H

#include <QtSql>
#include <QSqlDatabase>
#include "core/entities.h"
#include <vector>


void createDatabase(QSqlDatabase& db, QString& filename);

bool createDbConnection();


class PomodoroGateway
{
public:
    static QStringList getPomodorosForToday() {
        QStringList result;
        QSqlQuery query;
        query.exec("select id, name, start_time, finish_time "
               "from pomodoro where date(start_time) = date('now') "
               "order by start_time desc");
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


class TagGateway
{
public:
    static void insertTag(QString tag, QVariant associatedTodoItemId) {
        QSqlQuery query;
        query.prepare("select id from tag where name = (:name)");
        query.bindValue(":name", QVariant(tag));
        query.exec();
        QVariant tagId;
        if (query.next()) {
            tagId = query.value(0);
        } else {
            query.prepare("insert into tag (name) values (:name)");
            query.bindValue(":name", QVariant(tag));
            query.exec();
            tagId = query.lastInsertId();
        }

        query.prepare("insert into todotag (tag_id, todo_id) values (:tag_id, :todo_id)");
        query.bindValue(":tag_id", tagId);
        query.bindValue(":todo_id", associatedTodoItemId);
        query.exec();
    }

    static void removeTagIfOrphaned(QVariant tag_id) {
        QSqlQuery query;
        query.exec("pragma foreign_keys = on");
        query.prepare("select count(*) from todotag where todotag.tag_id = (:tag_id)");
        query.bindValue(":tag_id", tag_id);
        query.exec();
        if (query.next()) {
            int numTimesTagEncountered = query.value(0).toInt();
            if (numTimesTagEncountered > 1) {

            } else {
                query.prepare("delete from tag where tag.id = (:tag_id)");
                query.bindValue(":tag_id", tag_id);
                query.exec();
            }
        }
    }

    static QList<QVariant> getTagsForTodoItem(int id) {
        QSqlQuery query;
        query.prepare("select tag.id "
                "from todo_item "
                "join todotag on todo_item.id = todotag.todo_id "
                "join tag on tag.id = todotag.tag_id "
                "where todo_item.id = (:removed_item_id) ");
        query.bindValue(":removed_item_id", QVariant(id));
        query.exec();
        QList<QVariant> itemTags;
        while (query.next()) {
            itemTags << query.value(0);
        }
        return itemTags;
    }

    static QStringList getAllTags() {
        QStringList tags;
        QSqlQuery query;
        query.exec("select name from tag");
        while (query.next()) {
            tags << query.value(0).toString();
        }
        return tags;
    }
};


class TodoItemGateway
{
public:
    static int storeTodoItem(TodoItem item) {
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
        if (!item.tags.isEmpty()) {
            for (QString tag : item.tags) {
                TagGateway::insertTag(tag, todoId);
            }
        }
        return todoId.toInt();
    }

    static void removeTodoItem(int id) {
        QSqlQuery query;
        query.exec("pragma foreign_keys = on");
        QList<QVariant> removedItemTags = TagGateway::getTagsForTodoItem(id);
        for (QVariant tag_id : removedItemTags) {
            TagGateway::removeTagIfOrphaned(tag_id);
        }
        query.prepare("delete from todo_item where id = (:removed_item_id)");
        query.bindValue(":removed_item_id", id);
        query.exec();
    }

    static void updateTodoItem(TodoItem& updatedItem) {
        // Assumes that updatedItem has the same id as an old one
        QList<QVariant> oldItemTagsIds = TagGateway::getTagsForTodoItem(updatedItem.id);
        QSqlQuery query;
        query.prepare("update todo_item set name = (:name), "
               "estimated_pomodoros = (:estimated_pomodoros), "
               "last_modified = (:last_modified) "
               "where id = (:id)");
        query.bindValue(":name", QVariant(updatedItem.name));
        query.bindValue(":estimated_pomodoros", QVariant(updatedItem.estimatedPomodoros));
        query.bindValue(":last_modified", QVariant(QDateTime::currentDateTime()));
        query.bindValue(":id", QVariant(updatedItem.id));
        query.exec();

        for (QVariant tag_id : oldItemTagsIds) {
            TagGateway::removeTagIfOrphaned(tag_id);
            query.prepare("delete from todotag "
                   "where todotag.tag_id = (:tag_id) and todotag.todo_id = (:todo_id)");
            query.bindValue(":tag_id", tag_id);
            query.bindValue(":todo_id", QVariant(updatedItem.id));
            query.exec();
        }

        if (!updatedItem.tags.isEmpty()) {
            for (QString tag : updatedItem.tags) {
                TagGateway::insertTag(tag, updatedItem.id);
            }
        }
    }

    static void incrementSpentPomodoros(TodoItem& item) {
        QSqlQuery query;
        query.prepare("update todo_item set spent_pomodoros = (:spent_pomodoros) where id = (:todo_item_id)");
        query.bindValue(":todo_item_id", QVariant(item.id));
        query.bindValue(":spent_pomodoros", QVariant(item.spentPomodoros));
        query.exec();
    }

    static QList<TodoItem> getUncompleteTodoItems() {
        QSqlQuery query;
        query.exec("select todo_item.name, todo_item.estimated_pomodoros, "
               "todo_item.spent_pomodoros, todo_item.priority, todo_item.completed, group_concat(tag.name), todo_item.id "
               "from todo_item join todotag on todo_item.id = todotag.todo_id "
               "join tag on tag.id = todotag.tag_id "
               "where completed = 0 or todo_item.last_modified > DATETIME('NOW', '-1 DAY') "
               "group by todo_item.name");
        QList<TodoItem> items;
        while (query.next()) {
            QStringList tags = query.value(5).toString().split(",");
            TodoItem item {query.value(0).toString(),
                           query.value(1).toUInt(),
                           query.value(2).toUInt(),
                           query.value(3).toUInt(),
                           tags,
                           query.value(4).toBool(),
                           query.value(6).toInt()};
            items << item;
        }
        return items;
    }

    static void setItemChecked(int itemId, bool value) {
        QSqlQuery query;
        query.prepare("update todo_item set completed = (:completed), last_modified = (:last_modified) where id = (:id)");
        query.bindValue(":completed", QVariant(value));
        query.bindValue(":id", QVariant(itemId));
        query.bindValue(":last_modified", QVariant(QDateTime::currentDateTime()));
        query.exec();
    }

};

#endif // DB_HELPER_H
