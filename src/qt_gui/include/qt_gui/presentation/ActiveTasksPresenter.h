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
#ifndef ACTIVETASKSPRESENTER_H_IYZPDQWQ
#define ACTIVETASKSPRESENTER_H_IYZPDQWQ

#include "core/CommandHandler.h"
#include "core/QueryHandler.h"
#include "core/use_cases/TaskMapper.h"
#include "core/use_cases/change_tasks_priority/ChangeActiveTasksPriorityCommand.h"
#include "core/use_cases/delete_task/DeleteTaskCommand.h"
#include "core/use_cases/edit_task/EditTaskCommand.h"
#include "core/use_cases/request_tasks/ActiveTasksQuery.h"
#include "core/use_cases/toggle_task_completed/ToggleTaskCompletedCommand.h"
#include "qt_gui/presentation/TaskContract.h"

namespace sprint_timer::ui {

class ActiveTasksPresenter : public contracts::TaskContract::Presenter {
public:
    using active_tasks_hdl_t = QueryHandler<use_cases::ActiveTasksQuery>;
    using edit_task_hdl_t = CommandHandler<use_cases::EditTaskCommand>;
    using delete_task_hdl_t = CommandHandler<use_cases::DeleteTaskCommand>;
    using toggle_task_completion_hdl_t =
        CommandHandler<use_cases::ToggleTaskCompletedCommand>;
    using change_priority_hdl_t =
        CommandHandler<use_cases::ChangeActiveTasksPriorityCommand>;

    ActiveTasksPresenter(active_tasks_hdl_t& activeTasksHandler,
                         edit_task_hdl_t& editTaskHandler,
                         delete_task_hdl_t& deleteTaskHandler,
                         toggle_task_completion_hdl_t& toggleFinishedHandler,
                         change_priority_hdl_t& changePriorityHandler);

    void editTask(const use_cases::TaskDTO& editedTask) override;

    void deleteTask(const std::string& uuid) override;

    void toggleFinished(const std::string& uuid,
                        dw::DateTime lastModificationTimestamp) override;

    void reorderTasks(int32_t sourceRow,
                      int32_t count,
                      int32_t destinationRow) override;

private:
    active_tasks_hdl_t& activeTasksHandler;
    edit_task_hdl_t& editTaskHandler;
    delete_task_hdl_t& deleteTaskHandler;
    toggle_task_completion_hdl_t& toggleFinishedHandler;
    change_priority_hdl_t& changePriorityHandler;
    std::vector<use_cases::TaskDTO> tasks;

    void updateViewImpl() override;

    void onViewAttached() override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: ACTIVETASKSPRESENTER_H_IYZPDQWQ */
