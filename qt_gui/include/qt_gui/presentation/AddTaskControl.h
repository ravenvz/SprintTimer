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
#ifndef ADDTASKCONTROL_H_N0QTM94K
#define ADDTASKCONTROL_H_N0QTM94K

#include "qt_gui/presentation/BasePresenter.h"
#include <vector>

namespace sprint_timer::ui::contracts::AddTaskControl {

struct TaskDetails {
    std::string name;
    std::vector<std::string> tags;
    int estimatedCost;
    int actualCost;
};

class View {
public:
    virtual void displayAddTaskDialog(std::vector<std::string>&& tags) = 0;

    virtual ~View() = default;
};

class Presenter : public BasePresenter<View> {
public:
    virtual void onTaskAddConfirmed(const TaskDetails& details) const = 0;

    virtual void
    onTaskAddedInTextForm(const std::string& taskDescription) const = 0;

    virtual void onDialogRequested() const = 0;
};

} // namespace sprint_timer::ui::contracts::AddTaskControl

#endif /* end of include guard: ADDTASKCONTROL_H_N0QTM94K */
