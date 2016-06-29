#ifndef STATISTICSWIDGET_H
#define STATISTICSWIDGET_H

#include "core/IConfig.h"
#include "core/IPomodoroService.h"
#include "core/PomodoroStatistics.h"
#include "dialogs/datepickdialog.h"
#include "plot.h"
#include "timediagram.h"
#include "widgets/DataWidget.h"
#include "widgets/DistributionDiagram.h"
#include <memory>


using std::experimental::optional;

namespace Ui {
class StatisticsWidget;
}


/* Displays statistical information on completed Pomodoros
 * for a given timespan.
 *
 * Has following major parts:
 *      widget that allows to select timespan;
 *      widget that displays number of Pomodoros
 *      for each day in a timespan;
 *      widget that displays average number of Pomodoros for each
 *      weekday;
 *      widget that displays distribution of tags for all completed
 *      Pomodoros;
 *      widget that displays distribution of worktime.
 */
class StatisticsWidget : public DataWidget {
    Q_OBJECT

public:
    StatisticsWidget(IConfig& applicationSettings,
                     IPomodoroService& pomodoroService,
                     QWidget* parent = nullptr);
    ~StatisticsWidget();

    void synchronize();

private slots:
    void onDatePickerIntervalChanged(DateInterval newInterval);
    void onTagSelected(size_t tagIndex);

private:
    Ui::StatisticsWidget* ui;
    IConfig& applicationSettings;
    IPomodoroService& pomodoroService;
    std::vector<Pomodoro> pomodoros;
    TagDistribution tagDistribution;
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
    void updateWeekdayBarChart(const Distribution<double>& weekdayDistribution);
    void updateWeekdayBarChartLegend(
        const Distribution<double>& weekdayDistribution);
    void
    updateDailyTimelineGraph(const Distribution<double>& dailyDistribution);
    void updateDailyTimelineGraphLegend(
        const Distribution<double>& dailyDistribution);
    void
    updateWorkHoursDiagram(const Distribution<double>& workTimeDistribution,
                           const std::vector<Pomodoro>& pomodoros);
    void updateTopTagsDiagram(
        std::vector<TagCount>& tagTagCounts); // TODO rename it's a shame
    void onYearRangeUpdated(const std::vector<std::string>& yearRange);
    void onPomodorosFetched(const std::vector<Pomodoro>& pomodoros);
};


#endif /* end of include guard: STATISTICSWIDGET_H */
