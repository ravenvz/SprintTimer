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
#include "api/handlers/CreateTaskHandler.h"
#include "api/actions/CreateTask.h"

namespace {

using sprint_timer::api::CreateTaskCommand;
using sprint_timer::utils::and_then;
using sprint_timer::utils::transform;

} // namespace

namespace sprint_timer::api {

CreateTaskHandler::CreateTaskHandler(
    TaskStorage& taskStorage_,
    ActionInvoker& actionInvoker_,
    UUIDGenerator& uuidGenerator_,
    DateTimeProvider& dateTimeProvider_,
    const Converter<TaskDTO, Task>& taskMapper_)
    : taskStorage{taskStorage_}
    , actionInvoker{actionInvoker_}
    , uuidGenerator{uuidGenerator_}
    , dateTimeProvider{dateTimeProvider_}
    , taskMapper{taskMapper_}
{
}

auto CreateTaskHandler::handle(const CreateTaskCommand& command) -> void
{
    const auto dto = TaskDTO{uuidGenerator.generateUUID(),
                             command.tags,
                             command.name,
                             command.estimatedCost,
                             {},
                             false,
                             dateTimeProvider.dateTimeLocalNow(),
                             command.notes,
                             command.timeFrame,
                             command.type};
    const auto task = [&]() {
        const auto t = taskMapper(dto);
        if (not command.parent or command.timeFrame.due) {
            return t;
        }
        const auto matchingTasks =
            taskStorage.findByUuid(command.parent.value());
        if (matchingTasks.empty()) {
            // TODO throw proper exception
            throw std::runtime_error{"Throw handler exception here"};
        }
        const auto parentTask = matchingTasks.front();
        return t.inheritDate(parentTask, dateTimeProvider.dateTimeLocalNow());
    }();
    actionInvoker.execute(actions::CreateTask{
        taskStorage, task, command.parent, command.insertBeforePos});
}

} // namespace sprint_timer::api

