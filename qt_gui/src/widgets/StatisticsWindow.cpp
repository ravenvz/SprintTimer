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
#include "widgets/StatisticsWindow.h"
#include "core/utils/WeekdaySelection.h"
#include "ui_statistics_window.h"
#include "widgets/BarChart.h"

StatisticsWindow::StatisticsWindow(IConfig& applicationSettings,
                                   ICoreService& coreService,
                                   QWidget* parent)
    : DataWidget(parent)
    , ui(new Ui::StatisticsWindow)
    , applicationSettings(applicationSettings)
    , coreService{coreService}
{
    setAttribute(Qt::WA_DeleteOnClose);
    coreService.yearRange(
        [this](const auto& range) { this->onYearRangeUpdated(range); });
    ui->setupUi(this);
    currentInterval = ui->dateRangePicker->getInterval();

    connect(ui->dateRangePicker,
            &DateRangePicker::timeSpanChanged,
            this,
            &StatisticsWindow::onDatePickerIntervalChanged);
    connect(ui->topTagDiagram,
            &DistributionDiagram::chartSelectionChanged,
            this,
            &StatisticsWindow::onTagSelected);
}

StatisticsWindow::~StatisticsWindow() { delete ui; }

void StatisticsWindow::synchronize() { fetchData(); }

void StatisticsWindow::fetchData()
{
    coreService.sprintsInTimeRange(
        currentInterval.toTimeSpan(),
        [this](const auto& sprints) { this->onDataFetched(sprints); });
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

void StatisticsWindow::onYearRangeUpdated(
    const std::vector<std::string>& yearRange)
{
    ui->dateRangePicker->setYears(yearRange);
}

void StatisticsWindow::onDatePickerIntervalChanged(DateInterval newInterval)
{
    currentInterval = newInterval;
    fetchData();
}

void StatisticsWindow::drawGraphs()
{
    SprintStatItem statistics{
        selectedTagIndex ? tagTop.sprintsForTagAt(*selectedTagIndex) : sprints,
        currentInterval.toTimeSpan()};
    const WeekdaySelection workdays{applicationSettings.workdaysCode()};
    ui->dailyTimelineGraph->setData(
        statistics.dailyDistribution(),
        currentInterval.startDate,
        numWorkdays(currentInterval.toTimeSpan(), workdays),
        applicationSettings.dailyGoal());
    ui->bestWorkdayWidget->setData(statistics.weekdayDistribution());
    ui->bestWorktimeWidget->setData(statistics.worktimeDistribution(),
                                    statistics.sprints());
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
    ui->topTagDiagram->setLegendTitleFont(QFont(".Helvetica Neue Desk UI", 13));
}

void StatisticsWindow::onTagSelected(size_t tagIndex)
{
    if (!selectedTagIndex) {
        selectedTagIndex = tagIndex;
    }
    else {
        selectedTagIndex
            = (*selectedTagIndex == tagIndex) ? std::optional<size_t>() : tagIndex;
    }

    drawGraphs();
}
