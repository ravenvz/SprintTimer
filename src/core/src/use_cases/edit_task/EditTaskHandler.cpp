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
#include "core/use_cases/edit_task/EditTaskHandler.h"
#include "core/HandlerException.h"
#include "core/actions/EditTask.h"
#include <algorithm>

namespace sprint_timer::use_cases {

using entities::Tag;
using entities::Task;

EditTaskHandler::EditTaskHandler(TaskStorage& taskStorage_,
                                 ActionInvoker& actionInvoker_)
    : taskStorage{taskStorage_}
    , actionInvoker{actionInvoker_}
{
}

void EditTaskHandler::handle(EditTaskCommand&& command)
{
    const auto& editedDTO = command.editedTask;
    const auto& tagDTO = editedDTO.tags;
    auto matchingUuid = taskStorage.findByUuid(editedDTO.uuid);
    if (matchingUuid.empty()) {
        std::string message{"Trying to edit task with uuid: "};
        message += editedDTO.uuid;
        message += " that does not exist.";
        throw HandlerException{message};
    }
    std::list<Tag> tags;
    std::transform(cbegin(tagDTO),
                   cend(tagDTO),
                   std::back_inserter(tags),
                   [](const auto& elem) { return Tag{elem}; });
    Task originalTask = matchingUuid.front();
    Task editedTask{editedDTO.name,
                    editedDTO.expectedCost,
                    originalTask.actualCost(),
                    editedDTO.uuid,
                    tags,
                    originalTask.isCompleted(),
                    dw::current_date_time_local()};
    actionInvoker.execute(std::make_unique<actions::EditTask>(
        taskStorage, originalTask, editedTask));
}

} // namespace sprint_timer::use_cases
