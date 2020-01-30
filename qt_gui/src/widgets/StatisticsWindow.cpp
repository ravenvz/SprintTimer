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
#include "qt_gui/widgets/StatisticsWindow.h"
#include "qt_gui/widgets/DistributionDiagram.h"
#include "qt_gui/widgets/TimeDiagram.h"
#include <QVBoxLayout>

namespace sprint_timer::ui::qt_gui {

using namespace entities;

StatisticsWindow::StatisticsWindow(
    QueryHandler<use_cases::RequestSprintsQuery, std::vector<entities::Sprint>>&
        requestSprintsHandler_,
    dw::Weekday firstDayOfWeek,
    QAbstractItemModel& operationRangeModel_,
    const WorkScheduleWrapper& workScheduleWrapper_,
    DatasyncRelay& datasyncRelay_,
    QWidget* parent_)
    : SprintTimerWidget{parent_}
    , requestSprintsHandler{requestSprintsHandler_}
    , workScheduleWrapper{workScheduleWrapper_}
    , datasyncRelay{datasyncRelay_}
{
    auto layout = std::make_unique<QVBoxLayout>(nullptr);

    auto dateRangePicker_ =
        std::make_unique<DateRangePicker>(operationRangeModel_, firstDayOfWeek);
    dateRangePicker = dateRangePicker_.get();

    auto statisticsDiagramWidget_ = std::make_unique<StatisticsDiagramWidget>();
    statisticsDiagramWidget = statisticsDiagramWidget_.get();

    auto dailyTimelineGraph_ = std::make_unique<DailyTimelineGraph>();
    dailyTimelineGraph = dailyTimelineGraph_.get();

    layout->addWidget(dateRangePicker_.release(), 1);
    layout->addWidget(dailyTimelineGraph_.release(), 3);
    layout->addWidget(statisticsDiagramWidget_.release(), 4);
    setLayout(layout.release());

    connections.push_back(connect(&datasyncRelay_,
                                  &DatasyncRelay::dataUpdateRequiered,
                                  this,
                                  &StatisticsWindow::synchronize));
    connections.push_back(connect(statisticsDiagramWidget,
                                  &StatisticsDiagramWidget::tagSelectionChanged,
                                  this,
                                  &StatisticsWindow::onTagSelected));
    connections.push_back(
        connect(&workScheduleWrapper,
                &WorkScheduleWrapper::workScheduleChanged,
                [this](const WorkSchedule&) { updateViews(); }));
    connections.push_back(connect(dateRangePicker,
                                  &DateRangePicker::selectedDateRangeChanged,
                                  this,
                                  &StatisticsWindow::synchronize));

    synchronize();
}

StatisticsWindow::~StatisticsWindow()
{
    std::for_each(connections.begin(), connections.end(), [](auto& connection) {
        disconnect(connection);
    });
}

QSize StatisticsWindow::sizeHint() const { return QSize{1100, 730}; }

void StatisticsWindow::synchronize()
{
    onSprintsUpdated(requestSprintsHandler.handle(
        use_cases::RequestSprintsQuery{dateRangePicker->selectionRange()}));
}

void StatisticsWindow::onSprintsUpdated(
    const std::vector<Sprint>& updatedSprints)
{
    datasyncRelay.onSyncCompleted("StatisticsWindow");
    sprints = updatedSprints;
    selectedTagIndex = std::optional<size_t>();
    tagTop = TagTop(sprints, numTopTags);
    updateViews();
    statisticsDiagramWidget->setTagFrequencies(tagTop.tagFrequencies());
}

void StatisticsWindow::updateViews()
{
    const auto dateRange = dateRangePicker->selectionRange();
    const std::vector<Sprint>& interestingSprints =
        (selectedTagIndex ? tagTop.sprintsForTagAt(*selectedTagIndex)
                          : sprints);
    const auto dailyDistribution =
        dailyStatistics(interestingSprints, dateRange);
    const int workdays =
        numWorkdays(workScheduleWrapper.workSchedule(), dateRange);
    const int goal = goalFor(workScheduleWrapper.workSchedule(), dateRange);
    dailyTimelineGraph->setData(dailyStatistics(interestingSprints, dateRange),
                                dateRange,
                                workdays,
                                goal);
    statisticsDiagramWidget->setData(interestingSprints, dateRange);
}

void StatisticsWindow::onTagSelected(size_t tagIndex)
{
    if (!selectedTagIndex) {
        selectedTagIndex = tagIndex;
    }
    else {
        selectedTagIndex = (*selectedTagIndex == tagIndex)
                               ? std::optional<size_t>()
                               : tagIndex;
    }

    updateViews();
}

} // namespace sprint_timer::ui::qt_gui
