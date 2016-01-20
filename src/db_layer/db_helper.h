#ifndef DB_HELPER_H
#define DB_HELPER_H

#include <QtSql>
#include <QSqlDatabase>
#include <vector>
#include <algorithm>
#include "core/entities/TodoItem.h"
#include "core/entities/Pomodoro.h"
#include <QDebug>


bool createDatabase(QSqlDatabase& db, QString& filename);

bool createSchema();

bool createDbConnection();

bool activateForeignKeys();


class PomodoroDataSource
{
public:

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
                      "group by (strftime('%j', date(dt, '-3 days', 'weekday 4')) - 1) / 7 + 1 "
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

    static QStringList getStoredPomodorosYearsRange() {
        QStringList result;
        QSqlQuery query;
        query.exec("select distinct strftime('%Y', start_time) "
                   "from pomodoro order by start_time;");
        while (query.next()) {
            result << query.value(0).toString();
        }
        return result;
    }
};


class TodoItemDataSource
{
public:
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
                           query.value(4).toBool()};
            result << std::make_pair(item, query.value(7).toDate().toString());
        }
        return result;
    }

};

#endif // DB_HELPER_H
