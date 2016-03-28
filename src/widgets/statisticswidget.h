#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include <QVector>
#include "core/PomodoroStatistics.h"
#include "core/config.h"
#include "dialogs/datepickdialog.h"
#include "src/models/pomodoromodel.h"
#include "piediagram.h"
#include "plot.h"
#include "timediagram.h"

namespace Ui {
class StatisticsWidget;
}


class StatisticsWidget : public QWidget {
    Q_OBJECT

public:
    explicit StatisticsWidget(IConfig& applicationSettings,
                              QWidget* parent = 0);
    ~StatisticsWidget();
    void updateView();

private slots:
    void onDatePickerIntervalChanged(DateInterval newInterval);
    void onSliceSelectionChanged(int newSliceIndex);

private:
    Ui::StatisticsWidget* ui;
    IConfig& applicationSettings;
    PomodoroModel* pomodoroModel;
    QVector<Pomodoro> pomodoros;
    TagPomoMap tagPomoMap;
    TimeDiagram* workTimeDiagram;
    DateInterval currentInterval;
    const int numDisplayedTagSlices = 7; // TODO move to config
    int selectedSliceIndex = -1;

    void connectSlots();
    void setupGraphs();
    void fetchPomodoros();
    void drawGraphs();
    void setupWeekdayBarChart();
    void setupDailyTimelineGraph();
    void updateWeekdayBarChart(Distribution<double>* weekdayDistribution);
    void updateWeekdayBarChartLegend(Distribution<double>* weekdayDistribution);
    void updateDailyTimelineGraph(Distribution<double>* dailyDistribution);
    void
    updateDailyTimelineGraphLegend(Distribution<double>* dailyDistribution);
    void updateWorkHoursDiagram(Distribution<double>* workTimeDistribution,
                                const QVector<Pomodoro>& pomodoros);
    void updateTopTagsDiagram(QVector<Slice>& tagSlices);
};


#endif /* end of include guard: STATISTICSWIDGET_H */
