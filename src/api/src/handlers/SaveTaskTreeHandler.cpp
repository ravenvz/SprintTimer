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
#include "api/dtos/TaskTypeMapper.h"
#include "core/TaskMetadataTree.h"

namespace sprint_timer::api {

SaveTaskTreeHandler::SaveTaskTreeHandler(
    TaskStorage& taskStorage_,
    ActionInvoker& actionInvoker_,
    const Converter<TaskTreeDTO, TaskTree>& taskTreeMapper_)
    : taskStorage{taskStorage_}
    , actionInvoker{actionInvoker_}
    , taskTreeMapper{taskTreeMapper_}
{
}

void SaveTaskTreeHandler::handle(const SaveTaskTreeCommand& command)
{
    // // TODO wire invoker
    taskStorage.saveTree(taskTreeMapper(command.taskTree));
}

} // namespace sprint_timer::api
