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
#ifndef TESTCOMMANDHANDLERCOMPOSER_H_TCE5XWBA
#define TESTCOMMANDHANDLERCOMPOSER_H_TCE5XWBA

#include "core/ActionInvoker.h"
#include "core/SprintStorage.h"
#include "core/TaskStorage.h"
#include "core/TaskTreeMetadataStorage.h"
#include "core/use_cases/change_schedule/ChangeWorkScheduleHandler.h"
#include "core/use_cases/change_tasks_priority/ChangeActiveTasksPriorityHandler.h"
#include "core/use_cases/create_task/CreateTaskHandler.h"
#include "core/use_cases/delete_sprint/DeleteSprintHandler.h"
#include "core/use_cases/delete_task/DeleteTaskHandler.h"
#include "core/use_cases/edit_task/EditTaskHandler.h"
#include "core/use_cases/register_sprint/RegisterSprintBulkHandler.h"
#include "core/use_cases/rename_tag/RenameTagHandler.h"
#include "core/use_cases/save_task_tree/SaveTaskTreeHandler.h"
#include "core/use_cases/toggle_task_completed/ToggleTaskCompletedHandler.h"
#include "use_cases_tests/CommandHandlerComposer.h"

namespace sprint_timer::compose {

struct TestCommandHandlerComposer final : public CommandHandlerComposer {
    TestCommandHandlerComposer(ActionInvoker& actionInvoker_,
                               TaskStorage& taskStorage_,
                               SprintStorage& sprintStorage_,
                               TaskTreeMetadataStorage& taskTree_,
                               WorkScheduleWriter& scheduleStorage_,
                               UUIDGenerator& generator_,
                               DateTimeProvider& dateTimeProvider_);

    CommandHandler<use_cases::CreateTaskCommand>& createTaskHandler() override;

    CommandHandler<use_cases::DeleteTaskCommand>& deleteTaskHandler() override;

    CommandHandler<use_cases::RegisterSprintBulkCommand>&
    registerSprintBulkHandler() override;

    CommandHandler<use_cases::ToggleTaskCompletedCommand>&
    toggleTaskCompletedHandler() override;

    CommandHandler<use_cases::DeleteSprintCommand>&
    deleteSprintHandler() override;

    CommandHandler<use_cases::EditTaskCommand>& editTaskHandler() override;

    CommandHandler<use_cases::ChangeActiveTasksPriorityCommand>&
    reorderTasksHandler() override;

    CommandHandler<use_cases::RenameTagCommand>& renameTagHandler() override;

    CommandHandler<use_cases::ChangeWorkScheduleCommand>&
    changeWorkScheduleHandler() override;

    CommandHandler<use_cases::SaveTaskTreeCommand>&
    saveTaskTreeHandler() override;

private:
    std::unique_ptr<use_cases::CreateTaskHandler> createTask;
    use_cases::DeleteTaskHandler deleteTask;
    std::unique_ptr<use_cases::RegisterSprintBulkHandler> registerSprintBulk;
    use_cases::ToggleTaskCompletedHandler toggleTaskCompleted;
    use_cases::DeleteSprintHandler deleteSprint;
    use_cases::EditTaskHandler editTask;
    use_cases::ChangeActiveTasksPriorityHandler reorderTasks;
    use_cases::RenameTagHandler renameTag;
    use_cases::ChangeWorkScheduleHandler changeSchedule;
    use_cases::SaveTaskTreeHandler saveTaskTree;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: TESTCOMMANDHANDLERCOMPOSER_H_TCE5XWBA */
