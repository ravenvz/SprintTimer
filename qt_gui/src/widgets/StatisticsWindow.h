/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
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
class StatisticsWindow;
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
class StatisticsWindow : public DataWidget {
    Q_OBJECT

public:
    StatisticsWindow(IConfig& applicationSettings,
                     IPomodoroService& pomodoroService,
                     QWidget* parent = nullptr);
    ~StatisticsWindow();

    void synchronize() override;

private slots:
    void onDatePickerIntervalChanged(DateInterval newInterval);
    void onTagSelected(size_t tagIndex);

private:
    //    Ui::StatisticsWindow* ui;
    Ui::StatisticsWindow* ui;
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
    void updateWeekdayBarChart(const Distribution<double>& weekdayDistribution);
    void updateWeekdayBarChartLegend(
        const Distribution<double>& weekdayDistribution);
    void
    updateWorkHoursDiagram(const Distribution<double>& workTimeDistribution,
                           const std::vector<Pomodoro>& pomodoros);
    void updateTopTagsDiagram(
        std::vector<TagCount>& tagTagCounts); // TODO rename it's a shame
    void onYearRangeUpdated(const std::vector<std::string>& yearRange);
    void onPomodorosFetched(const std::vector<Pomodoro>& pomodoros);
};


#endif /* end of include guard: STATISTICSWIDGET_H */
