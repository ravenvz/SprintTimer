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
#ifndef COMMANDHANDLERCOMPOSER_H_TIYTGWRO
#define COMMANDHANDLERCOMPOSER_H_TIYTGWRO

#include "api/com_query/CommandHandler.h"
#include "api/requests/CancelWorkflowCommand.h"
#include "api/requests/ChangeActiveTasksPriorityCommand.h"
#include "api/requests/ChangeWorkScheduleCommand.h"
#include "api/requests/CreateTaskCommand.h"
#include "api/requests/DeleteSprintCommand.h"
#include "api/requests/DeleteTaskCommand.h"
#include "api/requests/EditTaskCommand.h"
#include "api/requests/RegisterSprintBulkCommand.h"
#include "api/requests/RenameTagCommand.h"
#include "api/requests/SaveTaskTreeCommand.h"
#include "api/requests/StartTimerCommand.h"
#include "api/requests/ToggleTaskCompletedCommand.h"
#include "api/requests/ToggleZoneCommand.h"
#include "api/requests/UndoLastCommand.h"

namespace sprint_timer::compose {

struct CommandHandlerComposer {
    virtual ~CommandHandlerComposer() = default;

    virtual auto
    createTaskHandler() -> asp::CommandHandler<api::CreateTaskCommand>& = 0;

    virtual auto
    deleteTaskHandler() -> asp::CommandHandler<api::DeleteTaskCommand>& = 0;

    virtual auto registerSprintBulkHandler()
        -> asp::CommandHandler<api::RegisterSprintBulkCommand>& = 0;

    virtual auto toggleTaskCompletedHandler()
        -> asp::CommandHandler<api::ToggleTaskCompletedCommand>& = 0;

    virtual auto
    deleteSprintHandler() -> asp::CommandHandler<api::DeleteSprintCommand>& = 0;

    virtual auto
    editTaskHandler() -> asp::CommandHandler<api::EditTaskCommand>& = 0;

    virtual auto reorderTasksHandler()
        -> asp::CommandHandler<api::ChangeActiveTasksPriorityCommand>& = 0;

    virtual auto
    renameTagHandler() -> asp::CommandHandler<api::RenameTagCommand>& = 0;

    virtual auto changeWorkScheduleHandler()
        -> asp::CommandHandler<api::ChangeWorkScheduleCommand>& = 0;

    virtual auto
    saveTaskTreeHandler() -> asp::CommandHandler<api::SaveTaskTreeCommand>& = 0;

    virtual auto
    undoHandler() -> asp::CommandHandler<api::UndoLastCommand>& = 0;

    virtual auto
    startTimerHandler() -> asp::CommandHandler<api::StartTimerCommand>& = 0;

    virtual auto cancelWorkflowHandler()
        -> asp::CommandHandler<api::CancelWorkflowCommand>& = 0;

    virtual auto
    toggleZoneHandler() -> asp::CommandHandler<api::ToggleZoneCommand>& = 0;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: COMMANDHANDLERCOMPOSER_H_TIYTGWRO */
