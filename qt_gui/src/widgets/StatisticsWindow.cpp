/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#include "qt_gui/widgets/BarChart.h"
#include "qt_gui/widgets/DistributionDiagram.h"
#include "qt_gui/widgets/Plot.h"
#include "qt_gui/widgets/TimeDiagram.h"
#include "ui_statistics_window.h"
#include <core/use_cases/RequestMinMaxYear.h>
#include <core/use_cases/RequestSprints.h>
#include <core/utils/WeekdaySelection.h>

namespace sprint_timer::ui::qt_gui {

using namespace entities;
using use_cases::RequestMinMaxYear;
using use_cases::RequestSprints;

StatisticsWindow::StatisticsWindow(const IConfig& applicationSettings,
                                   ISprintStorageReader& sprintReader,
                                   IYearRangeReader& sprintYearRangeReader,
                                   QueryInvoker& queryInvoker,
                                   QWidget* parent)
    : QFrame{parent}
    , ui{std::make_unique<Ui::StatisticsWindow>()}
    , applicationSettings{applicationSettings}
    , sprintReader{sprintReader}
    , sprintYearRangeReader{sprintYearRangeReader}
    , queryInvoker{queryInvoker}
{
    ui->setupUi(this);

    connect(ui->dateRangePicker,
            &DateRangePicker::timeSpanChanged,
            this,
            &StatisticsWindow::onDatePickerIntervalChanged);
    connect(ui->topTagDiagram,
            &DistributionDiagram::chartSelectionChanged,
            this,
            &StatisticsWindow::onTagSelected);

    queryInvoker.execute(std::make_unique<RequestMinMaxYear>(
        sprintYearRangeReader,
        [this](const auto& range) { ui->dateRangePicker->setYears(range); }));
}

StatisticsWindow::~StatisticsWindow() = default;

void StatisticsWindow::synchronize() { fetchData(); }

void StatisticsWindow::fetchData()
{
    queryInvoker.execute(std::make_unique<RequestSprints>(
        sprintReader,
        ui->dateRangePicker->getInterval().toTimeSpan(),
        [this](const auto& sprints) { this->onDataFetched(sprints); }));
}

void StatisticsWindow::onDataFetched(const std::vector<Sprint>& sprints)
{
    this->sprints = sprints;
    selectedTagIndex = std::optional<size_t>();
    tagTop = TagTop(sprints, numTopTags);
    std::vector<TagTop::TagFrequency> tagFrequency = tagTop.tagFrequencies();
    drawGraphs();
    updateTopTagsDiagram(tagFrequency);
}

void StatisticsWindow::onDatePickerIntervalChanged(DateInterval newInterval)
{
    fetchData();
}

void StatisticsWindow::drawGraphs()
{
    const auto interval = currentInterval();
    const auto timeSpan = interval.toTimeSpan();
    const std::vector<Sprint>& interestingSprints
        = (selectedTagIndex ? tagTop.sprintsForTagAt(*selectedTagIndex)
                            : sprints);
    SprintStatItem statistics{interestingSprints, timeSpan};
    const utils::WeekdaySelection workdays{applicationSettings.workdays()};
    ui->dailyTimelineGraph->setData(
        statistics.dailyDistribution(),
        interval.startDate,
        static_cast<int>(numWorkdays(timeSpan, workdays)),
        applicationSettings.dailyGoal());
    ui->bestWorkdayWidget->setData(statistics.weekdayDistribution());
    ui->bestWorktimeWidget->setData(statistics.worktimeDistribution(),
                                    interestingSprints);
}

void StatisticsWindow::updateTopTagsDiagram(
    std::vector<TagTop::TagFrequency>& tagCounts)
{
    if (!tagCounts.empty() && tagCounts.back().first == Tag{""})
        tagCounts.back().first.setName("others");
    std::vector<std::pair<std::string, double>> data;
    std::transform(tagCounts.cbegin(),
                   tagCounts.cend(),
                   std::back_inserter(data),
                   [](const auto& elem) -> std::pair<std::string, double> {
                       return {elem.first.name(), elem.second};
                   });
    ui->topTagDiagram->setData(data);
    ui->topTagDiagram->setLegendTitle("Top tags");
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

    drawGraphs();
}

DateInterval StatisticsWindow::currentInterval() const
{
    return ui->dateRangePicker->getInterval();
}

} // namespace sprint_timer::ui::qt_gui
