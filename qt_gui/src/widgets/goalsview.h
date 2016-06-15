#ifndef GOALSVIEW_H
#define GOALSVIEW_H

#include "core/IConfig.h"
#include "core/IPomodoroService.h"
#include "widgets/DataWidget.h"
#include <QGridLayout>
#include <QProgressBar>
#include <memory>

namespace Ui {
class GoalsView;
}

class GoalsView : public DataWidget {
    Q_OBJECT

public:
    GoalsView(IConfig& applicationSettings,
              IPomodoroService& pomodoroService,
              QWidget* parent = nullptr);
    ~GoalsView();
    void synchronize() final;

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
    IPomodoroService& pomodoroService;

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
