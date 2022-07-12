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

#include "api/handlers/ChangeActiveTasksPriorityHandler.h"
#include "api/handlers/ChangeWorkScheduleHandler.h"
#include "api/handlers/CreateTaskHandler.h"
#include "api/handlers/DeleteSprintHandler.h"
#include "api/handlers/DeleteTaskHandler.h"
#include "api/handlers/EditTaskHandler.h"
#include "api/handlers/RegisterSprintBulkHandler.h"
#include "api/handlers/RenameTagHandler.h"
#include "api/handlers/SaveTaskTreeHandler.h"
#include "api/handlers/ToggleTaskCompletedHandler.h"
#include "api_tests/CommandHandlerComposer.h"
#include "core/ActionInvoker.h"
#include "core/SprintStorage.h"
#include "core/TaskStorage.h"
#include "core/TaskTreeMetadataStorage.h"

namespace sprint_timer::compose {

struct TestCommandHandlerComposer final : public CommandHandlerComposer {
    TestCommandHandlerComposer(ActionInvoker& actionInvoker_,
                               TaskStorage& taskStorage_,
                               SprintStorage& sprintStorage_,
                               TaskTreeMetadataStorage& taskTree_,
                               WorkScheduleWriter& scheduleStorage_,
                               UUIDGenerator& generator_,
                               DateTimeProvider& dateTimeProvider_);

    asp::CommandHandler<api::CreateTaskCommand>& createTaskHandler() override;

    asp::CommandHandler<api::DeleteTaskCommand>& deleteTaskHandler() override;

    asp::CommandHandler<api::RegisterSprintBulkCommand>&
    registerSprintBulkHandler() override;

    asp::CommandHandler<api::ToggleTaskCompletedCommand>&
    toggleTaskCompletedHandler() override;

    asp::CommandHandler<api::DeleteSprintCommand>&
    deleteSprintHandler() override;

    asp::CommandHandler<api::EditTaskCommand>& editTaskHandler() override;

    asp::CommandHandler<api::ChangeActiveTasksPriorityCommand>&
    reorderTasksHandler() override;

    asp::CommandHandler<api::RenameTagCommand>& renameTagHandler() override;

    asp::CommandHandler<api::ChangeWorkScheduleCommand>&
    changeWorkScheduleHandler() override;

    asp::CommandHandler<api::SaveTaskTreeCommand>&
    saveTaskTreeHandler() override;

private:
    std::unique_ptr<api::CreateTaskHandler> createTask;
    api::DeleteTaskHandler deleteTask;
    std::unique_ptr<api::RegisterSprintBulkHandler> registerSprintBulk;
    api::ToggleTaskCompletedHandler toggleTaskCompleted;
    api::DeleteSprintHandler deleteSprint;
    api::EditTaskHandler editTask;
    api::ChangeActiveTasksPriorityHandler reorderTasks;
    api::RenameTagHandler renameTag;
    api::ChangeWorkScheduleHandler changeSchedule;
    api::SaveTaskTreeHandler saveTaskTree;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: TESTCOMMANDHANDLERCOMPOSER_H_TCE5XWBA */
