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
#include <QFrame>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <iostream>

namespace sprint_timer::ui::qt_gui {

StatisticsWindow::StatisticsWindow(
    std::unique_ptr<QWidget> dailyTimelineGraph_,
    std::unique_ptr<QWidget> bestWorkdayWidget_,
    std::unique_ptr<QWidget> bestWorktimeWidget_,
    std::unique_ptr<QWidget> distributionDiagram_,
    std::unique_ptr<QWidget> datePickerWidget_,
    QWidget* parent_)
    : StandaloneDisplayableWidget{parent_}
{
    auto bottomFrame = std::make_unique<QFrame>();
    bottomFrame->setFrameStyle(QFrame::StyledPanel | QFrame::Sunken);
    auto bottomFrameLayout = std::make_unique<QHBoxLayout>(nullptr);
    bottomFrameLayout->addWidget(bestWorkdayWidget_.release(), 1);
    bottomFrameLayout->addWidget(distributionDiagram_.release(), 2);
    bottomFrameLayout->addWidget(bestWorktimeWidget_.release(), 2);
    bottomFrame->setLayout(bottomFrameLayout.release());

    auto layout = std::make_unique<QVBoxLayout>(nullptr);
    layout->addWidget(datePickerWidget_.release(), 1);
    layout->addWidget(dailyTimelineGraph_.release(), 3);
    layout->addWidget(bottomFrame.release(), 4);
    setLayout(layout.release());
}

StatisticsWindow::~StatisticsWindow()
{
    std::cerr << "StatisticsWindow destroyed\n";
}

QSize StatisticsWindow::sizeHint() const { return QSize{1100, 730}; }

} // namespace sprint_timer::ui::qt_gui
