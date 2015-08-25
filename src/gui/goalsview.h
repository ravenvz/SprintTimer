#ifndef GOALSVIEW_H
#define GOALSVIEW_H

#include <QWidget>
#include "core/config.h"

namespace Ui {
    class GoalsView;
}

class GoalsView : public QWidget
{
    Q_OBJECT

public:
    explicit GoalsView(Config& applicationSettings, QWidget* parent = 0);
    ~GoalsView();

private slots:
    void updateDailyGoal();
    void updateWeeklyGoal();
    void updateMonthlyGoal();

private:
    Ui::GoalsView* ui;
    Config& applicationSettings;
    QVector<unsigned> lastThirty;
    QVector<unsigned> lastQuarter;
    QVector<unsigned> lastYear;
    unsigned lastThirtyTotal;
    unsigned lastQuarterTotal;
    unsigned lastYearTotal;

    void connectSlots();
    void displayData();
    void drawDiagrams();
    unsigned int computeTotal(QVector<unsigned>& pomodoroDistribution);
    QString computeAverage(unsigned total, unsigned size);
    QString computePercentage(unsigned total, unsigned size, unsigned goal);
};


#endif
