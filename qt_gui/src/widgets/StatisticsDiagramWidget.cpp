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
#include "qt_gui/widgets/StatisticsDiagramWidget.h"
#include <QHBoxLayout>

namespace sprint_timer::ui::qt_gui {

StatisticsDiagramWidget::StatisticsDiagramWidget(QFrame* parent)
    : QFrame{parent}
{
    setFrameShape(StyledPanel);
    setFrameShadow(Raised);

    auto bestWorkdayWidget_ = std::make_unique<BestWorkdayWidget>();
    auto tagDiagram_ = std::make_unique<DistributionDiagram>();
    auto bestWorktimeWidget_ = std::make_unique<BestWorktimeWidget>();

    bestWorkdayWidget = bestWorkdayWidget_.get();
    tagDiagram = tagDiagram_.get();
    bestWorktimeWidget = bestWorktimeWidget_.get();

    connect(tagDiagram,
            &DistributionDiagram::chartSelectionChanged,
            [this](size_t index) { emit tagSelectionChanged(index); });

    auto layout = std::make_unique<QHBoxLayout>(this);
    layout->addWidget(bestWorkdayWidget_.release(), 7);
    layout->addWidget(tagDiagram_.release(), 10);
    layout->addWidget(bestWorktimeWidget_.release(), 10);
    setLayout(layout.release());
}

void StatisticsDiagramWidget::setData(
    const std::vector<entities::Sprint>& sprints,
    const dw::DateRange& dateRange)
{
    bestWorkdayWidget->setData(sprints, dateRange);
    bestWorktimeWidget->setData(sprints);
}

void StatisticsDiagramWidget::setTagFrequencies(
    std::vector<TagTop::TagFrequency>&& tagFrequencies)
{
    tagDiagram->setData(std::move(tagFrequencies));
}

} // namespace sprint_timer::ui::qt_gui
