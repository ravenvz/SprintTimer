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
#ifndef TODAYSPRINTS_H_V0K2AYQR
#define TODAYSPRINTS_H_V0K2AYQR

#include "qt_gui/presentation/BasePresenter.h"
#include <core/entities/Sprint.h>
#include <core/entities/Task.h>

namespace sprint_timer::ui::contracts::TodaySprints {

class View {
public:
    virtual void displaySprints(const std::vector<entities::Sprint>&) = 0;

    virtual void
    displayAddSprintDialog(const std::vector<entities::Task>& activeTasks,
                           dw::Weekday firstDayOfWeek,
                           std::chrono::minutes sprintDuration) = 0;

    virtual ~View() = default;
    View() = default;
    View(const View& other) = delete;
    View& operator=(const View& other) = delete;
    View(View&& other) noexcept = delete;
    View& operator=(View&& other) noexcept = delete;
};

class Presenter : public BasePresenter<View> {
public:
    virtual void onSprintDelete(const entities::Sprint& sprint) = 0;
};

} // namespace sprint_timer::ui::contracts::TodaySprints

#endif /* end of include guard: TODAYSPRINTS_H_V0K2AYQR */
