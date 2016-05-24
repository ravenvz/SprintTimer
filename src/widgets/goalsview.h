#ifndef GOALSVIEW_H
#define GOALSVIEW_H

#include "core/config.h"
#include "core/use_cases/use_cases.h"
#include <QGridLayout>
#include <QProgressBar>
#include <QWidget>
#include <memory>

namespace Ui {
class GoalsView;
}

class GoalsView : public QWidget {
    Q_OBJECT

public:
    GoalsView(IConfig& applicationSettings,
              CoreApi::PomodoroCoreFacade& pomodoroService,
              QWidget* parent = 0);
    ~GoalsView();
    void updateView();

private slots:
    void updateDailyGoal(int newValue);
    void updateWeeklyGoal(int newValue);
    void updateMonthlyGoal(int newValue);
    void onDailyDistributionReceived(const Distribution<int>& lastDays);
    void onWeeklyDistributionReceived(const Distribution<int>& lastWeeks);
    void onMonthlyDistributionReceived(const Distribution<int>& lastMonths);

private:
    Ui::GoalsView* ui;
    IConfig& applicationSettings;
    const QColor targetGoalReached = QColor("#6baa15");
    const QColor overwork = Qt::red;
    const QColor workInProgress = Qt::gray;
    CoreApi::PomodoroCoreFacade& pomodoroService;

    void displayData();
    void displayDailyData();
    void displayWeeklyData();
    void displayMonthlyData();
    void drawPeriodDiagram(QGridLayout* layout,
                           const Distribution<int>& distribution,
                           int goal,
                           int rowNum,
                           int colNum);
    void clearDiagramLayout(QGridLayout* layout);
    void updateProgressBar(QProgressBar* bar, int goal, int value);
};

QString formatDecimal(double decimal);

double percentage(int chunk, int total);


#endif
