/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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

#include "qt_gui/Synchronizable.h"
#include "qt_gui/utils/DateInterval.h"
#include <QFrame>
#include <core/IConfig.h>
#include <core/ISprintStorageReader.h>
#include <core/IYearRangeReader.h>
#include <core/QueryInvoker.h>
#include <core/SprintStatistics.h>
#include <core/TagTop.h>
#include <memory>
#include <optional>

namespace Ui {
class StatisticsWindow;
} // namespace Ui

namespace sprint_timer::ui::qt_gui {

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
class StatisticsWindow : public QFrame, public Synchronizable {
    Q_OBJECT

public:
    StatisticsWindow(const IConfig& applicationSettings,
                     ISprintStorageReader& sprintReader,
                     IYearRangeReader& sprintYearRangeReader,
                     QueryInvoker& queryInvoker,
                     QWidget* parent = nullptr);
    ~StatisticsWindow() override;

    void synchronize() override;

private slots:
    void onDatePickerIntervalChanged(DateInterval newInterval);
    void onTagSelected(size_t tagIndex);

private:
    std::unique_ptr<Ui::StatisticsWindow> ui;
    const IConfig& applicationSettings;
    ISprintStorageReader& sprintReader;
    IYearRangeReader& sprintYearRangeReader;
    QueryInvoker& queryInvoker;
    std::vector<entities::Sprint> sprints;
    TagTop tagTop;
    const size_t numTopTags{7}; // TODO move to config
    std::optional<size_t> selectedTagIndex;

    void fetchData();
    void drawGraphs();
    void updateTopTagsDiagram(std::vector<TagTop::TagFrequency>& tagCounts);
    void onDataFetched(const std::vector<entities::Sprint>& sprints);
    DateInterval currentInterval() const;
};

} // namespace sprint_timer::ui::qt_gui


#endif /* end of include guard: STATISTICSWINDOW_H */
