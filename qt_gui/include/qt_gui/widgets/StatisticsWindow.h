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

#include "qt_gui/models/SprintModel.h"
#include "qt_gui/models/WorkdayTrackerModel.h"
#include "qt_gui/widgets/DailyTimelineGraph.h"
#include "qt_gui/widgets/DateRangePicker.h"
#include "qt_gui/widgets/StatisticsDiagramWidget.h"
#include <QFrame>
#include <core/SprintStatistics.h>
#include <core/TagTop.h>
#include <memory>
#include <optional>

namespace sprint_timer::ui::qt_gui {

class StatisticsWindow : public QFrame {
    Q_OBJECT

public:
    StatisticsWindow(
        std::unique_ptr<DateRangePicker> dateRangePicker,
        std::unique_ptr<DailyTimelineGraph> dailyTimelineGraph,
        std::unique_ptr<StatisticsDiagramWidget> statisticsDiagramWidget,
        const WorkdayTrackerModel& workdayTrackerModel,
        const SprintModel& sprintModel,
        QWidget* parent = nullptr);

    ~StatisticsWindow() override;

    QSize sizeHint() const override;

    void onTagSelected(size_t tagIndex);

private:
    DateRangePicker* dateRangePicker;
    DailyTimelineGraph* dailyTimelineGraph;
    StatisticsDiagramWidget* statisticsDiagramWidget;
    const WorkdayTrackerModel& workdayTrackerModel;
    std::vector<entities::Sprint> sprints;
    TagTop tagTop;
    const size_t numTopTags{7}; // TODO move to config
    std::optional<size_t> selectedTagIndex;

    void updateViews();

    void onSprintsUpdated(const std::vector<entities::Sprint>& sprints);
};

} // namespace sprint_timer::ui::qt_gui


#endif /* end of include guard: STATISTICSWINDOW_H */
