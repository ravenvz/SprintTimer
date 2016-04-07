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


using std::experimental::optional;

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
    void onTagSelected(size_t tagIndex);

private:
    Ui::StatisticsWidget* ui;
    IConfig& applicationSettings;
    PomodoroModel* pomodoroModel;
    std::vector<Pomodoro> pomodoros;
    TagPomoMap tagPomoMap;
    TimeDiagram* workTimeDiagram;
    DateInterval currentInterval;
    const int numTopTags = 7; // TODO move to config
    optional<size_t> selectedTagIndex;

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
                                const std::vector<Pomodoro>& pomodoros);
    void updateTopTagsDiagram(std::vector<TagCount>& tagTagCounts);
};


#endif /* end of include guard: STATISTICSWIDGET_H */
