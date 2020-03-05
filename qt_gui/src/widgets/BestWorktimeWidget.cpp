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
#include "ui_best_worktime_widget.h"
#include <core/SprintStatistics.h>

namespace sprint_timer::ui::qt_gui {

using namespace entities;

BestWorktimeWidget::BestWorktimeWidget(
    BasePresenter<contracts::DaytimeStatisticsContract::View>& presenter_,
    QWidget* parent_)
    : QWidget{parent_}
    , ui{std::make_unique<Ui::BestWorktimeWidget>()}
    , presenter{presenter_}
{
    ui->setupUi(this);
    presenter.attachView(*this);
}

BestWorktimeWidget::~BestWorktimeWidget() = default;

void BestWorktimeWidget::updateLegend(
    const contracts::DaytimeStatisticsContract::LegendData& data)
{
    ui->labelBestWorktimeName->setText(QString::fromStdString(data.periodName));
    ui->labelBestWorktimeHours->setText(
        QString::fromStdString(data.periodHours));
}

void BestWorktimeWidget::updateDiagram(
    const contracts::DaytimeStatisticsContract::DiagramData& data)
{
    ui->timeDiagram->setIntervals(data.timeRanges);
}

} // namespace sprint_timer::ui::qt_gui
