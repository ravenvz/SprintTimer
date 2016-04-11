#ifndef GOALSTATMODEL_H
#define GOALSTATMODEL_H

#include <vector>
#include <QSqlQuery>

class GoalStatModel {
public:
    GoalStatModel();

    // Return vector containing number of completed pomodoros
    // up to 30 days back.
    std::vector<int> itemsDaysBack();

    // Return vector containing number of completed pomodoros
    // up to 12 weeks back.
    std::vector<int> itemsWeeksBack();

    // Return vector containing number of completed pomodoros
    // up to 12 months back.
    std::vector<int> itemsMonthsBack();

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
