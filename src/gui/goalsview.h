#ifndef GOALSVIEW_H
#define GOALSVIEW_H

#include <QWidget>
#include <QProgressBar>
#include "core/config.h"
#include <QGridLayout>
#include <src/utils/MathUtils.h>
#include "core/PomodoroStatistics.h"

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
    GoalStatItem goalStatistics;
    const QColor targetGoalReached = QColor("#6baa15");
    const QColor overwork = Qt::red;
    const QColor workInProgress = Qt::gray;

    void connectSlots();
    void displayData();
    void drawDiagrams();
    void drawLastMonthDiagram();
    void drawLastQuarterDiagram();
    void drawLastYearDiagram();
    void clearDiagramLayout(QGridLayout* layout);
    void updateProgressBar(QProgressBar* bar, unsigned goal, int value);
    QString formatDecimal(double decimal) const;
};


#endif
