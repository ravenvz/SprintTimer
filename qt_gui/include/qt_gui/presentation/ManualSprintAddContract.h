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
#ifndef MANUALSPRINTADDCONTRACT_H_HPMCRJBF
#define MANUALSPRINTADDCONTRACT_H_HPMCRJBF

#include "qt_gui/presentation/BasePresenter.h"
#include <core/entities/Task.h>

namespace sprint_timer::ui::contracts::ManualSprintAddContract {

class View {
public:
    virtual void
    displayAddSprintDialog(const std::vector<entities::Task>& activeTasks,
                           dw::Weekday firstDayOfWeek,
                           std::chrono::minutes sprintDuration) = 0;

    virtual void setInteractive(bool interactive) = 0;

    View() = default;
    virtual ~View() = default;
    View(const View& other) = delete;
    View& operator=(const View& other) = delete;
    View(View&& other) noexcept = delete;
    View& operator=(View&& other) noexcept = delete;
};

class Presenter : public BasePresenter<View> {
public:
    virtual void onSprintAddRequested() = 0;

    virtual void onSprintAddConfirmed(size_t taskNumber,
                                      dw::DateTime firstSprintStart,
                                      size_t numSprints) = 0;
};

} // namespace sprint_timer::ui::contracts::ManualSprintAddContract

#endif /* end of include guard: MANUALSPRINTADDCONTRACT_H_HPMCRJBF */
