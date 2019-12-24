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
#include <core/use_cases/RequestSprints.h>

#include <QDebug>

namespace sprint_timer::ui::qt_gui {

using namespace entities;

StatisticsWindow::StatisticsWindow(
    std::unique_ptr<DateRangePicker> dateRangePicker_,
    std::unique_ptr<DailyTimelineGraph> dailyTimelineGraph_,
    std::unique_ptr<StatisticsDiagramWidget> statisticsDiagramWidget_,
    const WorkScheduleModel& workScheduleModel_,
    ISprintStorageReader& sprintReader_,
    QueryInvoker& queryInvoker_,
    DatasyncRelay& datasyncRelay_,
    QWidget* parent_)
    : QFrame{parent_}
    , dateRangePicker{dateRangePicker_.get()}
    , dailyTimelineGraph{dailyTimelineGraph_.get()}
    , statisticsDiagramWidget{statisticsDiagramWidget_.get()}
    , workScheduleModel{workScheduleModel_}
    , sprintReader{sprintReader_}
    , queryInvoker{queryInvoker_}
{
    auto layout = std::make_unique<QVBoxLayout>(nullptr);
    layout->addWidget(dateRangePicker_.release(), 1);
    layout->addWidget(dailyTimelineGraph_.release(), 3);
    layout->addWidget(statisticsDiagramWidget_.release(), 4);
    setLayout(layout.release());

    connect(&datasyncRelay_,
            &DatasyncRelay::dataUpdateRequiered,
            this,
            &StatisticsWindow::synchronize);
    connect(statisticsDiagramWidget,
            &StatisticsDiagramWidget::tagSelectionChanged,
            this,
            &StatisticsWindow::onTagSelected);
    connect(&workScheduleModel,
            &WorkScheduleModel::workScheduleChanged,
            [this](const WorkSchedule&) { updateViews(); });
    connect(dateRangePicker,
            &DateRangePicker::selectedDateRangeChanged,
            this,
            &StatisticsWindow::synchronize);
}

StatisticsWindow::~StatisticsWindow() = default;

QSize StatisticsWindow::sizeHint() const { return QSize{1100, 730}; }

void StatisticsWindow::synchronize()
{
    queryInvoker.execute(std::make_unique<use_cases::RequestSprints>(
        sprintReader,
        dateRangePicker->selectionRange(),
        [this](const auto& sprints) { onSprintsUpdated(sprints); }));
}

void StatisticsWindow::onSprintsUpdated(
    const std::vector<Sprint>& updatedSprints)
{
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
        numWorkdays(workScheduleModel.workSchedule(), dateRange);
    const int goal = goalFor(workScheduleModel.workSchedule(), dateRange);
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
