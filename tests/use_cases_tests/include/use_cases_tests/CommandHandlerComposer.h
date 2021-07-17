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

#include "core/CommandHandler.h"
#include "core/use_cases/change_schedule/ChangeWorkScheduleCommand.h"
#include "core/use_cases/change_tasks_priority/ChangeActiveTasksPriorityCommand.h"
#include "core/use_cases/create_task/CreateTaskCommand.h"
#include "core/use_cases/delete_sprint/DeleteSprintCommand.h"
#include "core/use_cases/delete_task/DeleteTaskCommand.h"
#include "core/use_cases/edit_task/EditTaskCommand.h"
#include "core/use_cases/register_sprint/RegisterSprintBulkCommand.h"
#include "core/use_cases/rename_tag/RenameTagCommand.h"
#include "core/use_cases/toggle_task_completed/ToggleTaskCompletedCommand.h"

namespace sprint_timer::compose {

struct CommandHandlerComposer {
    virtual ~CommandHandlerComposer() = default;

    virtual CommandHandler<use_cases::CreateTaskCommand>&
    createTaskHandler() = 0;

    virtual CommandHandler<use_cases::DeleteTaskCommand>&
    deleteTaskHandler() = 0;

    virtual CommandHandler<use_cases::RegisterSprintBulkCommand>&
    registerSprintBulkHandler() = 0;

    virtual CommandHandler<use_cases::ToggleTaskCompletedCommand>&
    toggleTaskCompletedHandler() = 0;

    virtual CommandHandler<use_cases::DeleteSprintCommand>&
    deleteSprintHandler() = 0;

    virtual CommandHandler<use_cases::EditTaskCommand>& editTaskHandler() = 0;

    virtual CommandHandler<use_cases::ChangeActiveTasksPriorityCommand>&
    reorderTasksHandler() = 0;

    virtual CommandHandler<use_cases::RenameTagCommand>& renameTagHandler() = 0;

    virtual CommandHandler<use_cases::ChangeWorkScheduleCommand>&
    changeWorkScheduleHandler() = 0;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: COMMANDHANDLERCOMPOSER_H_TIYTGWRO */
