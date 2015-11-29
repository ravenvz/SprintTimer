#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include <QWidget>
#include <QVector>
// #include <memory>
#include "core/PomodoroStatistics.h"
#include "core/config.h"
#include "gui/dialogs/datepickdialog.h"
#include "piediagram.h"
#include "plot.h"
#include "timediagram.h"

namespace Ui {
    class StatisticsWidget;
}


class StatisticsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit StatisticsWidget(Config& applicationSettings, QWidget* parent = 0);
    ~StatisticsWidget();

private slots:
    void onDatePickerIntervalChanged(DateInterval newInterval);
    void onSliceSelectionChanged(int newSliceIndex);

private:
    Ui::StatisticsWidget* ui;
    Config& applicationSettings;
    QVector<Pomodoro> pomodoros;
    TagPomoMap tagPomoMap;
    TimeDiagram* workTimeDiagram;
    DateInterval currentInterval;
    const int numDisplayedTagSlices = 16;
    int selectedSliceIndex = -1;
    // std::unique_ptr<Plot> dailyTimeline;

    void connectSlots();
    void setupGraphs();
    void fetchPomodoros();
    void drawGraphs();
    void setupWeekdayBarChart();
    void setupDailyTimelineGraph();
    void updateWeekdayBarChart(Distribution<double>* weekdayDistribution);
    void updateWeekdayBarChartLegend(Distribution<double>* weekdayDistribution);
    void updateDailyTimelineGraph(Distribution<double>* dailyDistribution);
    void updateDailyTimelineGraphLegend(Distribution<double>* dailyDistribution);
    void updateWorkHoursDiagram(Distribution<double>* workTimeDistribution, const QVector<Pomodoro>& pomodoros);
    void updateTopTagsDiagram(QVector<Slice>& tagSlices);
};



#endif /* end of include guard: STATISTICSWIDGET_H */
