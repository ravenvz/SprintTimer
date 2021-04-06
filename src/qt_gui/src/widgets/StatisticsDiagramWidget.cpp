/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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

StatisticsDiagramWidget::StatisticsDiagramWidget(
    std::unique_ptr<QWidget> bestWorkdayWidget_,
    std::unique_ptr<QWidget> tagDiagramWidget_,
    std::unique_ptr<QWidget> bestWorktimeWidget_,
    QFrame* parent)
    : QFrame{parent}
{
    setFrameShape(StyledPanel);
    setFrameShadow(Raised);

    auto layout = std::make_unique<QHBoxLayout>();
    layout->addWidget(bestWorkdayWidget_.release(), 7);
    layout->addWidget(tagDiagramWidget_.release(), 10);
    layout->addWidget(bestWorktimeWidget_.release(), 10);
    setLayout(layout.release());
}

} // namespace sprint_timer::ui::qt_gui
