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
#include "api/handlers/EditTaskHandler.h"
#include "api/HandlerException.h"
#include "api/actions/EditTask.h"
#include "core/utils/Algutils.h"
#include <algorithm>

#include <iostream>

namespace sprint_timer::api {

EditTaskHandler::EditTaskHandler(TaskStorage& taskStorage_,
                                 ActionInvoker& actionInvoker_,
                                 const Converter<TaskDTO, Task>& taskMapper_)
    : taskStorage{taskStorage_}
    , actionInvoker{actionInvoker_}
    , taskMapper{taskMapper_}
{
}

void EditTaskHandler::handle(const EditTaskCommand& command)
{
    const Task desiredTask{taskMapper(command.editedTask)};
    const auto matchingUuid = taskStorage.findByUuid(desiredTask.uuid());
    if (matchingUuid.empty()) {
        std::string message{"Trying to edit task with uuid: "};
        message += desiredTask.uuid();
        message += " that does not exist.";
        throw HandlerException{message};
    }

    const Task& originalTask = matchingUuid.front();
    const Task editedTask = originalTask.edit(desiredTask);

    std::cout << "Original task" << std::endl;
    std::cout << originalTask << std::endl;
    std::cout << "Desired task" << std::endl;
    std::cout << desiredTask << std::endl;
    std::cout << "Edited task" << std::endl;
    std::cout << editedTask << std::endl;

    actionInvoker.execute(std::make_unique<actions::EditTask>(
        taskStorage, originalTask, editedTask));
}

} // namespace sprint_timer::api

