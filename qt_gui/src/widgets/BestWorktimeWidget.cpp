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
#include "qt_gui/widgets/BestWorktimeWidget.h"
#include "qt_gui/widgets/TimeDiagram.h"
#include <QGridLayout>
#include <QLabel>
#include <core/SprintStatistics.h>

namespace sprint_timer::ui::qt_gui {

using namespace entities;

BestWorktimeWidget::BestWorktimeWidget(QWidget* parent_)
    : QWidget{parent_}
    , bestWorktimeName{std::make_unique<QLabel>().release()}
    , bestWorkHours{std::make_unique<QLabel>().release()}
    , timeDiagram{std::make_unique<TimeDiagram>(this).release()}
{
    auto layout_ = std::make_unique<QGridLayout>();

    auto title_ = std::make_unique<QLabel>("Best Worktime");

    QFont boldFont{bestWorktimeName->font()};
    boldFont.setBold(true);
    bestWorktimeName->setFont(boldFont);

    layout_->addWidget(title_.release(), 0, 0);
    layout_->addWidget(bestWorktimeName, 1, 0);
    layout_->addWidget(bestWorkHours, 2, 0);
    layout_->addWidget(timeDiagram, 0, 1, 20, 5);

    setLayout(layout_.release());
}

void BestWorktimeWidget::updateLegend(
    const contracts::DaytimeStatisticsContract::LegendData& data)
{
    bestWorktimeName->setText(QString::fromStdString(data.periodName));
    bestWorkHours->setText(QString::fromStdString(data.periodHours));
}

void BestWorktimeWidget::updateDiagram(
    const contracts::DaytimeStatisticsContract::DiagramData& data)
{
    timeDiagram->setIntervals(data.timeRanges);
}

} // namespace sprint_timer::ui::qt_gui
