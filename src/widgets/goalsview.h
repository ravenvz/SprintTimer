#ifndef GOALSVIEW_H
#define GOALSVIEW_H

#include "core/config.h"
#include "models/goalstatmodel.h"
#include <QGridLayout>
#include <QProgressBar>
#include <QWidget>
#include <src/utils/MathUtils.h>

namespace Ui {
class GoalsView;
}

class GoalsView : public QWidget {
    Q_OBJECT

public:
    explicit GoalsView(IConfig& applicationSettings, QWidget* parent = 0);
    ~GoalsView();
    void updateView();

private slots:
    void updateDailyGoal(int newValue);
    void updateWeeklyGoal(int newValue);
    void updateMonthlyGoal(int newValue);

private:
    Ui::GoalsView* ui;
    IConfig& applicationSettings;
    GoalStatModel goalStatModel;
    const QColor targetGoalReached = QColor("#6baa15");
    const QColor overwork = Qt::red;
    const QColor workInProgress = Qt::gray;

    void displayData();
    void displayDailyData();
    void displayWeeklyData();
    void displayMonthlyData();
    void drawPeriodDiagram(QGridLayout* layout,
                           Distribution<int>& distribution,
                           int goal,
                           int rowNum,
                           int colNum);
    void clearDiagramLayout(QGridLayout* layout);
    void updateProgressBar(QProgressBar* bar, int goal, int value);
    QString formatDecimal(double decimal) const;
};


#endif
