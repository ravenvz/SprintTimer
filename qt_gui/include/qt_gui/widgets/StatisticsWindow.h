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
#ifndef STATISTICSWINDOW_H
#define STATISTICSWINDOW_H

#include "qt_gui/widgets/StandaloneDisplayableWidget.h"
#include <memory>

namespace sprint_timer::ui::qt_gui {

class StatisticsWindow : public StandaloneDisplayableWidget {

public:
    StatisticsWindow(std::unique_ptr<QWidget> dailyTimelineGraph,
                     std::unique_ptr<QWidget> bestWorkdayWidget,
                     std::unique_ptr<QWidget> bestWorktimeWidget,
                     std::unique_ptr<QWidget> distributionDiagram,
                     std::unique_ptr<QWidget> datePickerWidget,
                     QWidget* parent = nullptr);

    ~StatisticsWindow() override;

    QSize sizeHint() const override;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: STATISTICSWINDOW_H */
