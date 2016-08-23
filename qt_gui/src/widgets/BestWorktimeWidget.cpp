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

#include "widgets/BestWorktimeWidget.h"
#include "ui_best_worktime_widget.h"
#include "core/SprintStatistics.h"

BestWorktimeWidget::BestWorktimeWidget(QWidget* parent)
    : QWidget{parent}
    , ui{new Ui::BestWorktimeWidget}
{
    ui->setupUi(this);
}

BestWorktimeWidget::~BestWorktimeWidget() { delete ui; }

void BestWorktimeWidget::setData(
    const Distribution<double>& workTimeDistribution,
    const std::vector<Sprint>& sprints)
{
    updateWorkHoursDiagram(workTimeDistribution, sprints);
}

void BestWorktimeWidget::updateWorkHoursDiagram(
    const Distribution<double>& workTimeDistribution,
    const std::vector<Sprint>& sprints)
{
    std::vector<TimeSpan> timeSpans;
    timeSpans.reserve(sprints.size());
    std::transform(sprints.cbegin(),
                   sprints.cend(),
                   std::back_inserter(timeSpans),
                   [](const auto& sprint) { return sprint.timeSpan(); });
    if (timeSpans.empty()) {
        ui->labelBestWorktimeName->setText("No data");
        ui->labelBestWorktimeHours->setText("");
    }
    else {
        auto maxValueBin
            = static_cast<unsigned>(workTimeDistribution.getMaxValueBin());
        ui->labelBestWorktimeName->setText(
            QString::fromStdString(DayPart::dayPartName(maxValueBin)));
        ui->labelBestWorktimeHours->setText(
            QString::fromStdString(DayPart::dayPartHours(maxValueBin)));
    }
    ui->timeDiagram->setIntervals(std::move(timeSpans));
}

