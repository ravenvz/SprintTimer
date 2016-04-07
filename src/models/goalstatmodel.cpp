#include "goalstatmodel.h"
#include <QDate>
#include <QVariant>


GoalStatModel::GoalStatModel()
{
    daysQuery.prepare("select count(pomodoro.id) "
                      "from calendar left join pomodoro "
                      "on date(start_time) = dt "
                      "where dt > date(:start_date) and dt <= date(:end_date)"
                      "group by date(dt) "
                      "order by dt");
    weeksQuery.prepare("select count(start_time) "
                       "from calendar left join pomodoro "
                       "on date(start_time) = dt "
                       "where dt > (:start_date) and dt <= (:end_date) "
                       "group by (strftime('%j', date(dt, '-3 days', 'weekday "
                       "4')) - 1) / 7 + 1 "
                       "order by dt");
    monthsQuery.prepare("select count(start_time) "
                        "from calendar left join pomodoro "
                        "on date(start_time) = dt "
                        "where dt > (:start_date) and dt <= (:end_date) "
                        "group by strftime('%m', dt) "
                        "order by dt");
}

std::vector<int> GoalStatModel::itemsDaysBack()
{
    QDate now = QDate::currentDate();
    daysQuery.bindValue(":start_date", now.addDays(-30));
    daysQuery.bindValue(":end_date", now);
    daysQuery.exec();
    std::vector<int> result;
    while (daysQuery.next()) {
        result.push_back(daysQuery.value(0).toInt());
    }
    return result;
}

std::vector<int> GoalStatModel::itemsWeeksBack()
{
    QDate now = QDate::currentDate();
    weeksQuery.bindValue(":start_date", now.addDays(-7 * 11 - now.dayOfWeek()));
    weeksQuery.bindValue(":end_date", now);
    weeksQuery.exec();
    std::vector<int> result;
    while (weeksQuery.next()) {
        result.push_back(weeksQuery.value(0).toInt());
    }
    return result;
}

std::vector<int> GoalStatModel::itemsMonthsBack()
{
    QDate now = QDate::currentDate();
    monthsQuery.bindValue(":start_date",
                          now.addMonths(-11).addDays(-now.day()));
    monthsQuery.bindValue(":end_date", now);
    monthsQuery.exec();
    std::vector<int> result;
    while (monthsQuery.next()) {
        result.push_back(monthsQuery.value(0).toInt());
    }
    return result;
}
