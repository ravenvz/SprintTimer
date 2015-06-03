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
        QVariant name(pomodoro.name);
        QVariant startTime(pomodoro.startTime);
        QVariant finishTime(pomodoro.finishTime);
        query.prepare("insert into pomodoro (name, start_time, finish_time) "
                      "values (:name, :start_time, :finish_time)");
        query.bindValue(":name", name);
        query.bindValue(":start_time", startTime);
        query.bindValue(":finish_time", finishTime);
        query.exec();
    }
};

#endif // DB_HELPER_H
