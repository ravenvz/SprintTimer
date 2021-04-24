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
#include "qt_gui/presentation/ActiveTasksPresenter.h"
#include "core/utils/Algutils.h"

namespace sprint_timer::ui {

using use_cases::TaskDTO;

ActiveTasksPresenter::ActiveTasksPresenter(
    active_tasks_hdl_t& activeTasksHandler_,
    edit_task_hdl_t& editTaskHandler_,
    delete_task_hdl_t& deleteTaskHandler_,
    toggle_task_completion_hdl_t& toggleFinishedHandler_,
    change_priority_hdl_t& changePriorityHandler_)
    : activeTasksHandler{activeTasksHandler_}
    , editTaskHandler{editTaskHandler_}
    , deleteTaskHandler{deleteTaskHandler_}
    , toggleFinishedHandler{toggleFinishedHandler_}
    , changePriorityHandler{changePriorityHandler_}
{
}

void ActiveTasksPresenter::editTask(const TaskDTO& editedTask)
{
    editTaskHandler.handle(use_cases::EditTaskCommand{editedTask});
}

void ActiveTasksPresenter::deleteTask(const std::string& uuid)
{
    deleteTaskHandler.handle(use_cases::DeleteTaskCommand{uuid});
}

void ActiveTasksPresenter::toggleFinished(
    const std::string& uuid, dw::DateTime lastModificationTimestamp)
{
    toggleFinishedHandler.handle(
        use_cases::ToggleTaskCompletedCommand{uuid, lastModificationTimestamp});
}

void ActiveTasksPresenter::reorderTasks(int32_t sourceRow,
                                        int32_t count,
                                        int32_t destinationRow)
{
    std::vector<std::string> oldOrder(tasks.size());
    std::transform(cbegin(tasks),
                   cend(tasks),
                   begin(oldOrder),
                   [](const auto& elem) { return elem.uuid; });

    // Offset is needed due to slide implementation
    int32_t offset{sourceRow < destinationRow ? 1 : 0};

    std::vector<std::string> newOrder = oldOrder;
    utils::slide(begin(newOrder) + sourceRow,
                 begin(newOrder) + sourceRow + count,
                 begin(newOrder) + destinationRow + offset);

    changePriorityHandler.handle(use_cases::ChangeActiveTasksPriorityCommand{
        std::move(oldOrder), std::move(newOrder)});
}

void ActiveTasksPresenter::updateViewImpl()
{
    if (auto v = view(); v) {
        tasks = activeTasksHandler.handle(use_cases::ActiveTasksQuery{});
        v.value()->displayTasks(tasks);
    }
}

void ActiveTasksPresenter::onViewAttached() { updateView(); }

} // namespace sprint_timer::ui
