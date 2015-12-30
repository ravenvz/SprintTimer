#ifndef DB_HELPER_H
#define DB_HELPER_H

#include <QtSql>
#include <QSqlDatabase>
#include <vector>
#include <algorithm>
#include "core/entities/TodoItem.h"
#include "core/entities/Pomodoro.h"


void createDatabase(QSqlDatabase& db, QString& filename);

bool createDbConnection();


class PomodoroDataSource
{
public:
    static QStringList getPomodorosForToday() {
        QStringList result;
        QSqlQuery query;
        QDate today = QDate::currentDate();
        QVector<Pomodoro> pomodoros = getPomodorosBetween(today, today);
        std::transform(pomodoros.begin(), pomodoros.end(), std::back_inserter(result), [](const auto& pomo) {
                return pomo.toString();
            });
        return result;
    }

    static QVector<Pomodoro> getPomodorosBetween(const QDate& startDate, const QDate& endDate) {
        QVector<Pomodoro> result;
        QSqlQuery query;
        const int todoIdCol = 1;
        const int nameCol = 2;
        const int startTimeCol = 3;
        const int finishTimeCol = 4;
        const int tagsCol = 5;
        query.prepare("select pomodoro.id, pomodoro.todo_id, todo_item.name, start_time, finish_time, group_concat(tag.name) "
                      "from pomodoro "
                      "join todo_item on pomodoro.todo_id = todo_item.id "
                      "left join todotag on todo_item.id = todotag.todo_id "
                      "left join tag on todotag.tag_id = tag.id "
                      "where date(start_time) >= (:start_date) and date(start_time) <= (:end_date) "
                      "group by pomodoro.id;");
        query.bindValue(":start_date", QVariant(startDate));
        query.bindValue(":end_date", QVariant(endDate));
        query.exec();
        while (query.next()) {
            QString rawTags = query.value(tagsCol).toString();
            QStringList tags;
            if (!rawTags.isEmpty()) {
                tags = rawTags.split(",");
            }
            Pomodoro pomodoro {query.value(nameCol).toString(),
                               query.value(startTimeCol).toDateTime(),
                               query.value(finishTimeCol).toDateTime(),
                               tags,
                               query.value(todoIdCol).toInt()};
            result.append(pomodoro);
        }
        return result;
    }

    static QVector<unsigned> getPomodorosForLastThirtyDays() {
        QVector<unsigned> result;
        QSqlQuery query;
        query.exec("select count(start_time) "
                   "from calendar left join pomodoro "
                   "on date(start_time) = dt "
                   "where dt > date('now', '-30 days') and dt <= date('now') "
                   "group by date(dt) "
                   "order by dt");
        while (query.next()) {
            result << query.value(0).toUInt();
        }
        return result;
    }

    static QVector<unsigned> getPomodorosForLastTwelveWeeks() {
        QVector<unsigned> result;
        QDate today = QDate::currentDate();
        QDate startDate = today.addDays(-7*11 - today.dayOfWeek());
        QSqlQuery query;
        query.prepare("select count(start_time) "
                      "from calendar left join pomodoro "
                      "on date(start_time) = dt "
                      "where dt > (:start_date) and dt <= (:end_date) "
                      "group by strftime('%W', dt) "
                      "order by dt");
        query.bindValue(":start_date", QVariant(startDate));
        query.bindValue(":end_date", QVariant(today));
        query.exec();
        while (query.next()) {
            result << query.value(0).toUInt();
        }
        return result;
    }

    static QVector<unsigned> getPomodorosForLastTwelveMonths() {
        QVector<unsigned> result;
        QDate today = QDate::currentDate();
        QDate startDate = today.addMonths(-11).addDays(-today.day());
        QSqlQuery query;
        query.prepare("select count(start_time) "
                      "from calendar left join pomodoro "
                      "on date(start_time) = dt "
                      "where dt > (:start_date) and dt <= (:end_date) "
                      "group by strftime('%m', dt) "
                      "order by dt");
        query.bindValue(":start_date", QVariant(startDate));
        query.bindValue(":end_date", QVariant(today));
        query.exec();
        while (query.next()) {
            result << query.value(0).toUInt();
        }
        return result;
    }

    static QVector<Pomodoro> getPomodorosForLastQuarter() {
        QDate today = QDate::currentDate();
        QDate thirtyDaysAgo = today.addMonths(-3);
        return PomodoroDataSource::getPomodorosBetween(thirtyDaysAgo, today);
    }

    static void storePomodoro(Pomodoro pomodoro) {
        QSqlQuery query;
        query.prepare("insert into pomodoro (name, start_time, finish_time) "
                      "values (:name, :start_time, :finish_time)");
        query.bindValue(":name", QVariant(pomodoro.getName()));
        query.bindValue(":start_time", QVariant(pomodoro.getStartTime()));
        query.bindValue(":finish_time", QVariant(pomodoro.getFinishTime()));
        query.exec();
    }
};


class TagDataSource {
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


class TodoItemDataSource
{
public:
    static int storeTodoItem(const TodoItem& item) {
        const int newItemPriority = 10000;
        QSqlQuery query;
        query.prepare("insert into todo_item (name, estimated_pomodoros, spent_pomodoros, completed, priority, last_modified) "
                "values (:name, :estimated_pomodoros, :spent_pomodoros, :completed, :priority, :last_modified)");
        query.bindValue(":name", QVariant(item.getName()));
        query.bindValue(":estimated_pomodoros", QVariant(item.getEstimatedPomodoros()));
        query.bindValue(":spent_pomodoros", QVariant(item.getSpentPomodoros()));
        query.bindValue(":completed", QVariant(item.isCompleted()));
        query.bindValue(":priority", QVariant(newItemPriority));
        query.bindValue(":last_modified", QVariant(QDateTime::currentDateTime()));
        query.exec();
        QVariant todoId = query.lastInsertId();
        const QStringList& tags = item.getTags();
        if (!tags.isEmpty()) {
            for (QString tag : tags) {
                TagDataSource::insertTag(tag, todoId);
            }
        }
        return todoId.toInt();
    }

    static void removeTodoItem(int id) {
        QSqlQuery query;
        query.exec("pragma foreign_keys = on");
        QList<QVariant> removedItemTags = TagDataSource::getTagsForTodoItem(id);
        for (QVariant tag_id : removedItemTags) {
            TagDataSource::removeTagIfOrphaned(tag_id);
        }
        query.prepare("delete from todo_item where id = (:removed_item_id)");
        query.bindValue(":removed_item_id", id);
        query.exec();
    }

    static void updateTodoItem(TodoItem& updatedItem) {
        // Assumes that updatedItem has the same id as an old one
        QList<QVariant> oldItemTagsIds = TagDataSource::getTagsForTodoItem(updatedItem.getId());
        QSqlQuery query;
        query.prepare("update todo_item set name = (:name), "
               "estimated_pomodoros = (:estimated_pomodoros), "
               "last_modified = (:last_modified), "
               "completed = (:completed) "
               "where id = (:id)");
        query.bindValue(":name", QVariant(updatedItem.getName()));
        query.bindValue(":estimated_pomodoros", QVariant(updatedItem.getEstimatedPomodoros()));
        query.bindValue(":last_modified", QVariant(QDateTime::currentDateTime()));
        query.bindValue(":completed", QVariant(updatedItem.isCompleted()));
        query.bindValue(":id", QVariant(updatedItem.getId()));
        query.exec();

        for (QVariant tag_id : oldItemTagsIds) {
            TagDataSource::removeTagIfOrphaned(tag_id);
            query.prepare("delete from todotag "
                   "where todotag.tag_id = (:tag_id) and todotag.todo_id = (:todo_id)");
            query.bindValue(":tag_id", tag_id);
            query.bindValue(":todo_id", QVariant(updatedItem.getId()));
            query.exec();
        }

        const QStringList& tags = updatedItem.getTags();
        if (!tags.isEmpty()) {
            for (QString tag : tags) {
                TagDataSource::insertTag(tag, updatedItem.getId());
            }
        }
    }

    static void incrementSpentPomodoros(TodoItem& item) {
        QSqlQuery query;
        query.prepare("update todo_item set spent_pomodoros = (:spent_pomodoros) where id = (:todo_item_id)");
        query.bindValue(":todo_item_id", QVariant(item.getId()));
        query.bindValue(":spent_pomodoros", QVariant(item.getSpentPomodoros()));
        query.exec();
    }

    static QList<TodoItem> getUncompleteTodoItems() {
        QSqlQuery query;
        query.exec("SELECT todo_item.name, todo_item.estimated_pomodoros, "
               "todo_item.spent_pomodoros, todo_item.priority, todo_item.completed, group_concat(tag.name), todo_item.id "
               "FROM todo_item LEFT JOIN todotag ON todo_item.id = todotag.todo_id "
               "LEFT JOIN tag ON tag.id = todotag.tag_id "
               "WHERE completed = 0 OR todo_item.last_modified > DATETIME('NOW', '-1 DAY') "
               "GROUP BY todo_item.id "
               "ORDER BY todo_item.priority");
        QList<TodoItem> items;
        while (query.next()) {
            QStringList tags = query.value(5).toString().split(",");
            TodoItem item {query.value(0).toString(),
                           query.value(1).toUInt(),
                           query.value(2).toUInt(),
                           tags,
                           query.value(4).toBool(),
                           query.value(6).toInt()};
            items << item;
        }
        return items;
    }

    static QVector<std::pair<TodoItem, QString> > getTodoItemsForMonth(const QDate& startDate, const QDate& endDate) {
        QVector<std::pair<TodoItem, QString> > result;
        QSqlQuery query;
        query.prepare("SELECT todo_item.name, todo_item.estimated_pomodoros, "
                           "todo_item.spent_pomodoros, todo_item.priority, todo_item.completed, group_concat(tag.name), "
                           "todo_item.id, todo_item.last_modified "
                           "FROM todo_item LEFT JOIN todotag ON todo_item.id = todotag.todo_id "
                           "LEFT JOIN tag ON tag.id = todotag.tag_id "
                           "WHERE completed = 1 and todo_item.last_modified >= (:start_date) and "
                           "todo_item.last_modified <= (:end_date) "
                           "GROUP BY todo_item.id "
                           "ORDER BY todo_item.priority");
        query.bindValue(":start_date", QVariant(startDate));
        query.bindValue(":end_date", QVariant(endDate));
        query.exec();
        while (query.next()) {
            QStringList tags = query.value(5).toString().split(",");
            TodoItem item {query.value(0).toString(),
                           query.value(1).toUInt(),
                           query.value(2).toUInt(),
                           tags,
                           query.value(4).toBool(),
                           query.value(6).toInt()};
            result << std::make_pair(item, query.value(7).toDate().toString());
        }
        return result;
    }

    static void setItemChecked(int itemId, bool value) {
        QSqlQuery query;
        query.prepare("update todo_item set completed = (:completed), last_modified = (:last_modified) where id = (:id)");
        query.bindValue(":completed", QVariant(value));
        query.bindValue(":id", QVariant(itemId));
        query.bindValue(":last_modified", QVariant(QDateTime::currentDateTime()));
        query.exec();
    }

    static void setItemsPriority(const QList<TodoItem>& items) {
        QSqlDatabase::database().transaction();
        for (int i = 0; i < items.size(); ++i) {
            QSqlQuery query;
            query.prepare("update todo_item set priority = (:priority) where id = (:id)");
            query.bindValue(":priority", QVariant(i));
            query.bindValue(":id", QVariant(items[i].getId()));
            query.exec();
        }
        QSqlDatabase::database().commit();
    }

};

#endif // DB_HELPER_H
