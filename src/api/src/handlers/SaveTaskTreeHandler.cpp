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
#include "api/handlers/SaveTaskTreeHandler.h"
#include "api/dtos/TaskTreeMapper.h"

namespace sprint_timer::api {

SaveTaskTreeHandler::SaveTaskTreeHandler(
    TaskTreeMetadataStorage& taskTreeStorage_, ActionInvoker& actionInvoker_)
    : taskTreeStorage{taskTreeStorage_}
    , actionInvoker{actionInvoker_}
{
}

void SaveTaskTreeHandler::handle(const SaveTaskTreeCommand& command)
{
    // TODO wire invoker
    // const auto taskTree = fromDTO(command.taskTree);
    auto mapper = [](const TaskNodeDTO& node) {
        return TaskMetadata{
            node.task.uuid,
            static_cast<TaskType>(node.type), // TODO do proper mapping
            node.dueTime,
            node.reminder,
            node.notes};
    };
    const auto metadataTree = command.taskTree.mapped<TaskMetadata>(mapper);
    taskTreeStorage.saveTree(metadataTree);
}

} // namespace sprint_timer::api
