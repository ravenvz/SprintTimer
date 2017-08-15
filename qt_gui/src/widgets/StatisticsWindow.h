/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef STATISTICSWINDOW_H
#define STATISTICSWINDOW_H

#include "core/IConfig.h"
#include "core/ICoreService.h"
#include "core/SprintStatistics.h"
#include "dialogs/DateRangePickDialog.h"
#include "Plot.h"
#include "TimeDiagram.h"
#include "widgets/DataWidget.h"
#include "widgets/DistributionDiagram.h"
#include "core/TagTop.h"
#include "core/utils/OptionalPl.h"


namespace Ui {
class StatisticsWindow;
} // namespace Ui


/* Displays statistical information on finished sprints
 * for a given timespan.
 *
 * Has following major parts:
 *      widget that allows to select timespan;
 *      widget that displays number of sprints
 *      for each day in a timespan;
 *      widget that displays average number of sprints for each
 *      weekday;
 *      widget that displays distribution of tags for all completed
 *      sprints;
 *      widget that displays distribution of worktime.
 */
class StatisticsWindow : public DataWidget {
    Q_OBJECT

public:
    StatisticsWindow(IConfig& applicationSettings,
                     ICoreService& coreService,
                     QWidget* parent = nullptr);
    ~StatisticsWindow();

    void synchronize() override;

private slots:
    void onDatePickerIntervalChanged(DateInterval newInterval);
    void onTagSelected(size_t tagIndex);

private:
    Ui::StatisticsWindow* ui;
    IConfig& applicationSettings;
    ICoreService& coreService;
    std::vector<Sprint> sprints;
    TagTop tagTop;
    DateInterval currentInterval;
    const size_t numTopTags{7}; // TODO move to config
    optional<size_t> selectedTagIndex;

    void fetchData();
    void drawGraphs();
    void updateTopTagsDiagram(
        std::vector<TagTop::TagFrequency>& tagCounts);
    void onYearRangeUpdated(const std::vector<std::string>& yearRange);
    void onDataFetched(const std::vector<Sprint> &sprints);
};


#endif /* end of include guard: STATISTICSWINDOW_H */
