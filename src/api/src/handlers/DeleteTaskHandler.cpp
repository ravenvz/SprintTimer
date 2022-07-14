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
#include "api/handlers/DeleteTaskHandler.h"
#include "api/dtos/TaskMapper.h"
#include "core/HandlerException.h"
#include "core/actions/DeleteTask.h"

#include <iostream>

namespace sprint_timer::api {

DeleteTaskHandler::DeleteTaskHandler(TaskStorage& taskStorage_,
                                     ActionInvoker& actionInvoker_)
    : taskStorage{taskStorage_}
    , actionInvoker{actionInvoker_}
{
}

void DeleteTaskHandler::handle(const DeleteTaskCommand& command)
{
    auto matchingUuid = taskStorage.findByUuid(command.uuid);
    std::cout << "Task in handler: " << std::endl;
    std::cout << matchingUuid.front() << std::endl;
    if (matchingUuid.empty()) {
        std::string message{"Trying to delete task with uuid: "};
        message += command.uuid;
        message += " that doesn't exist.";
        throw HandlerException{message};
    }

    actionInvoker.execute(std::make_unique<actions::DeleteTask>(
        taskStorage, matchingUuid.front()));
}

} // namespace sprint_timer::api

