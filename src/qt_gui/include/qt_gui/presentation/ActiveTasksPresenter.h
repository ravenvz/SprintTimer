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

#include "api/com_query/CommandHandler.h"
#include "api/com_query/QueryHandler.h"
#include "api/dtos/TaskMapper.h"
#include "api/requests/ActiveTasksQuery.h"
#include "api/requests/ChangeActiveTasksPriorityCommand.h"
#include "api/requests/DeleteTaskCommand.h"
#include "api/requests/EditTaskCommand.h"
#include "api/requests/ToggleTaskCompletedCommand.h"
#include "qt_gui/presentation/TaskContract.h"
#include <optional>

namespace sprint_timer::ui {

class ActiveTasksPresenter : public contracts::TaskContract::Presenter {
public:
    using active_tasks_hdl_t = asp::QueryHandler<api::ActiveTasksQuery>;
    using edit_task_hdl_t = asp::CommandHandler<api::EditTaskCommand>;
    using delete_task_hdl_t = asp::CommandHandler<api::DeleteTaskCommand>;
    using toggle_task_completion_hdl_t =
        asp::CommandHandler<api::ToggleTaskCompletedCommand>;
    using change_priority_hdl_t =
        asp::CommandHandler<api::ChangeActiveTasksPriorityCommand>;

    ActiveTasksPresenter(active_tasks_hdl_t& activeTasksHandler,
                         edit_task_hdl_t& editTaskHandler,
                         delete_task_hdl_t& deleteTaskHandler,
                         toggle_task_completion_hdl_t& toggleFinishedHandler,
                         change_priority_hdl_t& changePriorityHandler);

    void editTask(const api::TaskDTO& editedTask) override;

    void deleteTask(const std::string& uuid) override;

    void toggleFinished(const std::string& uuid) override;

    void reorderTasks(int32_t sourceRow,
                      int32_t count,
                      int32_t destinationRow) override;

private:
    active_tasks_hdl_t& activeTasksHandler;
    edit_task_hdl_t& editTaskHandler;
    delete_task_hdl_t& deleteTaskHandler;
    toggle_task_completion_hdl_t& toggleFinishedHandler;
    change_priority_hdl_t& changePriorityHandler;
    std::optional<api::ActiveTasksQuery::Result> data;

    void fetchDataImpl() override;

    void updateViewImpl() override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: ACTIVETASKSPRESENTER_H_IYZPDQWQ */
