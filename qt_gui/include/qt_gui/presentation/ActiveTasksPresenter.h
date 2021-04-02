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
#ifndef ACTIVETASKSPRESENTER_H_IYZPDQWQ
#define ACTIVETASKSPRESENTER_H_IYZPDQWQ

#include "qt_gui/presentation/TaskContract.h"
#include "qt_gui/presentation/TaskMapper.h"
#include <core/CommandHandler.h>
#include <core/QueryHandler.h>
#include <core/use_cases/change_tasks_priority/ChangeUnfinishedTasksPriorityCommand.h>
#include <core/use_cases/delete_task/DeleteTaskCommand.h>
#include <core/use_cases/edit_task/EditTaskCommand.h>
#include <core/use_cases/request_tasks/UnfinishedTasksQuery.h>
#include <core/use_cases/toggle_task_completed/ToggleTaskCompletedCommand.h>
#include <core/utils/Algutils.h>

namespace sprint_timer::ui {

class ActiveTasksPresenter : public contracts::TaskContract::Presenter {
public:
    using active_tasks_hdl_t = QueryHandler<use_cases::UnfinishedTasksQuery,
                                            std::vector<entities::Task>>;
    using edit_task_hdl_t = CommandHandler<use_cases::EditTaskCommand>;
    using delete_task_hdl_t = CommandHandler<use_cases::DeleteTaskCommand>;
    using toggle_task_completion_hdl_t =
        CommandHandler<use_cases::ToggleTaskCompletedCommand>;
    using change_priority_hdl_t =
        CommandHandler<use_cases::ChangeUnfinishedTasksPriorityCommand>;

    ActiveTasksPresenter(active_tasks_hdl_t& activeTasksHandler,
                         edit_task_hdl_t& editTaskHandler,
                         delete_task_hdl_t& deleteTaskHandler,
                         toggle_task_completion_hdl_t& toggleFinishedHandler,
                         change_priority_hdl_t& changePriorityHandler);

    void editTask(const TaskDTO& oldTask, const TaskDTO& newTask) override;

    void deleteTask(const TaskDTO& task) override;

    void toggleFinished(const TaskDTO& task) override;

    void reorderTasks(size_t sourceRow,
                      size_t count,
                      size_t destinationRow) override;

private:
    active_tasks_hdl_t& activeTasksHandler;
    edit_task_hdl_t& editTaskHandler;
    delete_task_hdl_t& deleteTaskHandler;
    toggle_task_completion_hdl_t& toggleFinishedHandler;
    change_priority_hdl_t& changePriorityHandler;
    std::vector<TaskDTO> tasks;

    void updateViewImpl() override;

    void onViewAttached() override;
};

inline ActiveTasksPresenter::ActiveTasksPresenter(
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

inline void ActiveTasksPresenter::editTask(const TaskDTO& original,
                                           const TaskDTO& edited)
{
    editTaskHandler.handle(
        use_cases::EditTaskCommand{fromDTO(original), fromDTO(edited)});
}

inline void ActiveTasksPresenter::deleteTask(const TaskDTO& task)
{
    deleteTaskHandler.handle(use_cases::DeleteTaskCommand{fromDTO(task)});
}

inline void ActiveTasksPresenter::toggleFinished(const TaskDTO& task)
{
    toggleFinishedHandler.handle(
        use_cases::ToggleTaskCompletedCommand{fromDTO(task)});
}

inline void ActiveTasksPresenter::reorderTasks(size_t sourceRow,
                                               size_t count,
                                               size_t destinationRow)
{
    std::vector<std::string> oldOrder(tasks.size());
    std::transform(cbegin(tasks),
                   cend(tasks),
                   begin(oldOrder),
                   [](const auto& elem) { return elem.uuid; });

    // Offset is needed due to slide implementation
    int offset{sourceRow < destinationRow ? 1 : 0};

    std::vector<std::string> newOrder = oldOrder;
    utils::slide(begin(newOrder) + sourceRow,
                 begin(newOrder) + sourceRow + count,
                 begin(newOrder) + destinationRow + offset);

    changePriorityHandler.handle(
        use_cases::ChangeUnfinishedTasksPriorityCommand{std::move(oldOrder),
                                                        std::move(newOrder)});
}

inline void ActiveTasksPresenter::updateViewImpl()
{
    if (auto v = view(); v) {
        tasks = makeDTOs(
            activeTasksHandler.handle(use_cases::UnfinishedTasksQuery{}));
        v.value()->displayTasks(tasks);
    }
}

inline void ActiveTasksPresenter::onViewAttached() { updateView(); }

} // namespace sprint_timer::ui

#endif /* end of include guard: ACTIVETASKSPRESENTER_H_IYZPDQWQ */
