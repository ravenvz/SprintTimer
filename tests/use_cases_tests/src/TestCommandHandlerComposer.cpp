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
#include "use_cases_tests/TestCommandHandlerComposer.h"

namespace sprint_timer::compose {

TestCommandHandlerComposer::TestCommandHandlerComposer(
    ActionInvoker& actionInvoker_,
    TaskStorage& taskStorage_,
    SprintStorage& sprintStorage_,
    WorkScheduleWriter& scheduleStorage_,
    UUIDGenerator& generator_,
    DateTimeProvider& dateTimeProvider_)
    : createTask{std::make_unique<use_cases::CreateTaskHandler>(
          taskStorage_, actionInvoker_, generator_, dateTimeProvider_)}
    , deleteTask{taskStorage_, actionInvoker_}
    , registerSprintBulk{std::make_unique<use_cases::RegisterSprintBulkHandler>(
          taskStorage_, sprintStorage_, actionInvoker_, generator_)}
    , toggleTaskCompleted{taskStorage_, actionInvoker_}
    , deleteSprint{sprintStorage_, actionInvoker_}
    , editTask{taskStorage_, actionInvoker_}
    , reorderTasks{taskStorage_, actionInvoker_}
    , renameTag{taskStorage_, actionInvoker_}
    , changeSchedule{scheduleStorage_, actionInvoker_}
{
}

CommandHandler<use_cases::CreateTaskCommand>&
TestCommandHandlerComposer::createTaskHandler()
{
    return *createTask;
}

CommandHandler<use_cases::DeleteTaskCommand>&
TestCommandHandlerComposer::deleteTaskHandler()
{
    return deleteTask;
}

CommandHandler<use_cases::RegisterSprintBulkCommand>&
TestCommandHandlerComposer::registerSprintBulkHandler()
{
    return *registerSprintBulk;
}

CommandHandler<use_cases::ToggleTaskCompletedCommand>&
TestCommandHandlerComposer::toggleTaskCompletedHandler()
{
    return toggleTaskCompleted;
}

CommandHandler<use_cases::DeleteSprintCommand>&
TestCommandHandlerComposer::deleteSprintHandler()
{
    return deleteSprint;
}

CommandHandler<use_cases::EditTaskCommand>&
TestCommandHandlerComposer::editTaskHandler()
{
    return editTask;
}

CommandHandler<use_cases::ChangeActiveTasksPriorityCommand>&
TestCommandHandlerComposer::reorderTasksHandler()
{
    return reorderTasks;
}

CommandHandler<use_cases::RenameTagCommand>&
TestCommandHandlerComposer::renameTagHandler()
{
    return renameTag;
}

CommandHandler<use_cases::ChangeWorkScheduleCommand>&
TestCommandHandlerComposer::changeWorkScheduleHandler()
{
    return changeSchedule;
}

} // namespace sprint_timer::compose
