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
#include "api/dtos/SprintMapper.h"
#include "api/dtos/TagMapper.h"
#include "api/dtos/TaskMapper.h"
#include <algorithm>

namespace {

} // namespace

namespace sprint_timer::api {

EditTaskHandler::EditTaskHandler(TaskStorage& taskStorage_,
                                 ActionInvoker& actionInvoker_)
    : taskStorage{taskStorage_}
    , actionInvoker{actionInvoker_}
{
}

void EditTaskHandler::handle(const EditTaskCommand& command)
{
    const auto& editedDTO = command.editedTask;
    auto matchingUuid = taskStorage.findByUuid(editedDTO.uuid);
    if (matchingUuid.empty()) {
        std::string message{"Trying to edit task with uuid: "};
        message += editedDTO.uuid;
        message += " that does not exist.";
        throw HandlerException{message};
    }
    std::vector<Tag> tags;
    std::ranges::copy(dtoAdapter(editedDTO.tags), std::back_inserter(tags));
    const Task& originalTask = matchingUuid.front();
    const Task editedTask{editedDTO.name,
                          editedDTO.expectedCost,
                          std::vector<Sprint>{cbegin(originalTask.sprints()),
                                              cend(originalTask.sprints())},
                          editedDTO.uuid,
                          tags,
                          originalTask.isCompleted(),
                          dw::current_date_time_local()};
    actionInvoker.execute(std::make_unique<actions::EditTask>(
        taskStorage, originalTask, editedTask));
}

} // namespace sprint_timer::api

namespace {

} // namespace
