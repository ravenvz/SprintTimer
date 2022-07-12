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
#include "api/requests/ChangeWorkScheduleCommand.h"
#include "api/requests/ChangeActiveTasksPriorityCommand.h"
#include "api/requests/CreateTaskCommand.h"
#include "api/requests/DeleteSprintCommand.h"
#include "api/requests/DeleteTaskCommand.h"
#include "api/requests/EditTaskCommand.h"
#include "api/requests/RegisterSprintBulkCommand.h"
#include "api/requests/RenameTagCommand.h"
#include "api/requests/SaveTaskTreeCommand.h"
#include "api/requests/ToggleTaskCompletedCommand.h"

namespace sprint_timer::compose {

struct CommandHandlerComposer {
    virtual ~CommandHandlerComposer() = default;

    virtual asp::CommandHandler<api::CreateTaskCommand>&
    createTaskHandler() = 0;

    virtual asp::CommandHandler<api::DeleteTaskCommand>&
    deleteTaskHandler() = 0;

    virtual asp::CommandHandler<api::RegisterSprintBulkCommand>&
    registerSprintBulkHandler() = 0;

    virtual asp::CommandHandler<api::ToggleTaskCompletedCommand>&
    toggleTaskCompletedHandler() = 0;

    virtual asp::CommandHandler<api::DeleteSprintCommand>&
    deleteSprintHandler() = 0;

    virtual asp::CommandHandler<api::EditTaskCommand>& editTaskHandler() = 0;

    virtual asp::CommandHandler<api::ChangeActiveTasksPriorityCommand>&
    reorderTasksHandler() = 0;

    virtual asp::CommandHandler<api::RenameTagCommand>& renameTagHandler() = 0;

    virtual asp::CommandHandler<api::ChangeWorkScheduleCommand>&
    changeWorkScheduleHandler() = 0;

    virtual asp::CommandHandler<api::SaveTaskTreeCommand>&
    saveTaskTreeHandler() = 0;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: COMMANDHANDLERCOMPOSER_H_TIYTGWRO */
