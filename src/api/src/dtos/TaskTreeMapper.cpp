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
#include "api/dtos/TaskTreeMapper.h"
#include "api/dtos/TaskMapper.h"
#include "api/dtos/TaskTypeMapper.h"

namespace sprint_timer::api {

auto makeDTO(const TaskTree& taskTree) -> TaskTreeDTO
{
    auto mapNode = [](const TaskNode& taskNode) {
        return TaskNodeDTO{makeDTO(taskNode.task), makeDTO(taskNode.type)};
    };
    return taskTree.mapped<TaskNodeDTO>(mapNode);
}

auto fromDTO(const TaskTreeDTO& taskTreeDto) -> TaskTree
{
    auto mapNode = [](const TaskNodeDTO& taskNode) {
        return TaskNode{fromDTO(taskNode.task), fromDTO(taskNode.type)};
    };
    return taskTreeDto.mapped<TaskNode>(mapNode);
}

} // namespace sprint_timer::api
