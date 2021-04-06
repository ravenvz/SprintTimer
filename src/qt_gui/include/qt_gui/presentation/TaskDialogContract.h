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
#ifndef TASKDIALOGCONTRACT_H_NIV0BLMP
#define TASKDIALOGCONTRACT_H_NIV0BLMP

#include "qt_gui/presentation/BasePresenter.h"
#include <list>

namespace sprint_timer::ui::contracts::TaskDialog {

struct TaskDTO {
    std::string name;
    std::list<std::string> tags;
    int estimatedCost;
    int actualCost;
};

class View {
public:
    View() = default;
    virtual ~View() = default;

    virtual void fillForm(TaskDTO&& taskData) const = 0;

    virtual TaskDTO task() const = 0;

    View(View&&) = delete;
    View& operator=(View&&) = delete;

    View(const View&) = delete;
    View& operator=(const View&) = delete;
};

class Presenter : public BasePresenter<View> {
public:
};

} // namespace sprint_timer::ui::contracts::TaskDialog

#endif /* end of include guard: TASKDIALOGCONTRACT_H_NIV0BLMP */
