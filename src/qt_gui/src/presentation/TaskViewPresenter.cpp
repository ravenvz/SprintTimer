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
#include "qt_gui/presentation/TaskViewPresenter.h"
#include "cpp_utils/algorithms/optional_ext.h"

namespace sprint_timer::ui {

TaskViewPresenter::TaskViewPresenter(
    TaskSelectionMediator& taskSelectionMediator_)
    : taskSelectionMediator{taskSelectionMediator_}
{
    taskSelectionMediator.addColleague(this);
}

TaskViewPresenter::~TaskViewPresenter()
{
    taskSelectionMediator.removeColleague(this);
}

void TaskViewPresenter::changeTaskSelection(api::TaskDTO&& task)
{
    taskSelectionMediator.changeSelection(this, std::move(task));
}

void TaskViewPresenter::updateViewImpl() { onTaskSelectionChanged(); }

void TaskViewPresenter::onTaskSelectionChanged()
{
    alg::inspect(view(), [&](auto* view) {
        view->selectTask(taskSelectionMediator.taskUuid());
    });
}

} // namespace sprint_timer::ui
