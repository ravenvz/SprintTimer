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
#include "api_tests/TestCommandHandlerComposer.h"

namespace sprint_timer::compose {

TestCommandHandlerComposer::TestCommandHandlerComposer(
    ActionInvoker& actionInvoker_,
    TaskStorage& taskStorage_,
    SprintStorage& sprintStorage_,
    WorkScheduleWriter& scheduleStorage_,
    api::UUIDGenerator& generator_,
    api::DateTimeProvider& dateTimeProvider_,
    const patterns::Converter<api::TaskDTO, Task>& taskMapper_,
    // const Converter<std::string, Tag>& tagMapper_,
    const patterns::Converter<dw::DateTimeRange, Sprint>& sprintMapper_,
    const patterns::Converter<api::TaskTreeDTO, TaskTree>& taskTreeMapper_)
    : createTask{std::make_unique<api::CreateTaskHandler>(taskStorage_,
                                                          actionInvoker_,
                                                          generator_,
                                                          dateTimeProvider_,
                                                          taskMapper_)}
    , deleteTask{taskStorage_, actionInvoker_}
    , registerSprintBulk{std::make_unique<api::RegisterSprintBulkHandler>(
          taskStorage_, sprintStorage_, actionInvoker_, sprintMapper_)}
    , toggleTaskCompleted{taskStorage_,
                          actionInvoker_,
                          generator_,
                          dateTimeProvider_}
    , deleteSprint{sprintStorage_, actionInvoker_}
    , editTask{taskStorage_, actionInvoker_, dateTimeProvider_, taskMapper_}
    , reorderTasks{taskStorage_, actionInvoker_}
    , renameTag{taskStorage_, actionInvoker_}
    , changeSchedule{scheduleStorage_, actionInvoker_}
    , saveTaskTree{taskStorage_, actionInvoker_, taskTreeMapper_}
    , undo{actionInvoker_}
{
}

asp::CommandHandler<api::CreateTaskCommand>&
TestCommandHandlerComposer::createTaskHandler()
{
    return *createTask;
}

asp::CommandHandler<api::DeleteTaskCommand>&
TestCommandHandlerComposer::deleteTaskHandler()
{
    return deleteTask;
}

asp::CommandHandler<api::RegisterSprintBulkCommand>&
TestCommandHandlerComposer::registerSprintBulkHandler()
{
    return *registerSprintBulk;
}

asp::CommandHandler<api::ToggleTaskCompletedCommand>&
TestCommandHandlerComposer::toggleTaskCompletedHandler()
{
    return toggleTaskCompleted;
}

asp::CommandHandler<api::DeleteSprintCommand>&
TestCommandHandlerComposer::deleteSprintHandler()
{
    return deleteSprint;
}

asp::CommandHandler<api::EditTaskCommand>&
TestCommandHandlerComposer::editTaskHandler()
{
    return editTask;
}

asp::CommandHandler<api::ChangeActiveTasksPriorityCommand>&
TestCommandHandlerComposer::reorderTasksHandler()
{
    return reorderTasks;
}

asp::CommandHandler<api::RenameTagCommand>&
TestCommandHandlerComposer::renameTagHandler()
{
    return renameTag;
}

asp::CommandHandler<api::ChangeWorkScheduleCommand>&
TestCommandHandlerComposer::changeWorkScheduleHandler()
{
    return changeSchedule;
}

asp::CommandHandler<api::SaveTaskTreeCommand>&
TestCommandHandlerComposer::saveTaskTreeHandler()
{
    return saveTaskTree;
}

asp::CommandHandler<api::UndoLastCommand>&
TestCommandHandlerComposer::undoHandler()
{
    return undo;
}

} // namespace sprint_timer::compose
