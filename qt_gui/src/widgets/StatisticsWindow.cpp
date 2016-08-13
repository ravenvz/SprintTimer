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
#include "widgets/StatisticsWindow.h"
#include "ui_statistics_window.h"
#include "widgets/BarChart.h"


StatisticsWindow::StatisticsWindow(IConfig& applicationSettings,
                                   IPomodoroService& pomodoroService,
                                   QWidget* parent)
    : DataWidget(parent)
    , ui(new Ui::StatisticsWindow)
    , applicationSettings(applicationSettings)
    , pomodoroService{pomodoroService}
{
    setAttribute(Qt::WA_DeleteOnClose);
    pomodoroService.pomodoroYearRange(std::bind(
        &StatisticsWindow::onYearRangeUpdated, this, std::placeholders::_1));
    ui->setupUi(this);
    currentInterval = ui->widgetPickPeriod->getInterval();

    connect(ui->widgetPickPeriod,
            &PickPeriodWidget::timeSpanChanged,
            this,
            &StatisticsWindow::onDatePickerIntervalChanged);
    connect(ui->topTagDiagram,
            &DistributionDiagram::chartSelectionChanged,
            this,
            &StatisticsWindow::onTagSelected);
}

StatisticsWindow::~StatisticsWindow() { delete ui; }

void StatisticsWindow::synchronize() { fetchPomodoros(); }


void StatisticsWindow::fetchPomodoros()
{
    pomodoroService.pomodorosInTimeRange(
        currentInterval.toTimeSpan(),
        std::bind(&StatisticsWindow::onPomodorosFetched,
                  this,
                  std::placeholders::_1));
}

void StatisticsWindow::onPomodorosFetched(
    const std::vector<Pomodoro>& pomodoros)
{
    this->pomodoros = pomodoros;
    selectedTagIndex = optional<size_t>();
    tagDistribution = TagDistribution(pomodoros, numTopTags);
    std::vector<TagCount> tagTagCounts = tagDistribution.topTagsDistribution();
    drawGraphs();
    updateTopTagsDiagram(tagTagCounts);
}

void StatisticsWindow::onYearRangeUpdated(
    const std::vector<std::string>& yearRange)
{
    ui->widgetPickPeriod->setYears(yearRange);
}

void StatisticsWindow::onDatePickerIntervalChanged(DateInterval newInterval)
{
    currentInterval = newInterval;
    fetchPomodoros();
}

void StatisticsWindow::drawGraphs()
{
    PomodoroStatItem statistics{
        selectedTagIndex
            ? tagDistribution.pomodorosForNthTopTag(*selectedTagIndex)
            : pomodoros,
        currentInterval.toTimeSpan()};
    ui->dailyTimelineGraph->setData(statistics.dailyDistribution(),
                                    currentInterval.startDate,
                                    applicationSettings.dailyPomodorosGoal());
    ui->bestWorkdayWidget->setData(statistics.weekdayDistribution());
    ui->bestWorktimeWidget->setData(statistics.worktimeDistribution(),
                                    statistics.pomodoros());
}


void StatisticsWindow::updateTopTagsDiagram(std::vector<TagCount>& tagCounts)
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
            = (*selectedTagIndex == tagIndex) ? optional<size_t>() : tagIndex;
    }

    drawGraphs();
}
