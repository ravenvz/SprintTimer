#ifndef GOALSTATMODEL_H
#define GOALSTATMODEL_H

#include <QVector>
#include <QSqlQuery>

class GoalStatModel
{
public:
    GoalStatModel();

    // Return vector containing number of completed pomodoros
    // up to 30 days back.
    QVector<int> itemsDaysBack();

    // Return vector containing number of completed pomodoros
    // up to 12 weeks back.
    QVector<int> itemsWeeksBack();

    // Return vector containing number of completed pomodoros
    // up to 12 months back.
    QVector<int> itemsMonthsBack();

    // Return number of pomodoros completed today.
    int itemsCurrentDay();

    // Return number of pomodoros completed this week.
    int itemsCurrentWeek();

    // Return number of pomodoros completed this month.
    int itemsCurrentMonth();


private:
    QSqlQuery daysQuery;
    QSqlQuery weeksQuery;
    QSqlQuery monthsQuery;
};


#endif /* end of include guard: GOALSTATMODEL_H */
