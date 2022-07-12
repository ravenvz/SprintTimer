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

namespace {

sprint_timer::api::TaskTypeDTO makeTaskTypeDTO(sprint_timer::TaskType taskType)
{
    using namespace sprint_timer::api;
    using enum sprint_timer::TaskType;

    switch (taskType) {
    case Project:
        return TaskTypeDTO::Project;
    case Folder:
        return TaskTypeDTO::Folder;
    case Recurring:
        return TaskTypeDTO::Recurring;
    case Regular:
        return TaskTypeDTO::Regular;
    };

    return TaskTypeDTO::Regular;
}

sprint_timer::TaskType
fromTaskTypeDTO(const sprint_timer::api::TaskTypeDTO& taskType)
{
    using namespace sprint_timer;
    using enum sprint_timer::api::TaskTypeDTO;

    switch (taskType) {
    case Project:
        return TaskType::Project;
        break;
    case Folder:
        return TaskType::Folder;
    case Recurring:
        return TaskType::Recurring;
    case Regular:
        return TaskType::Regular;
    };

    return TaskType::Regular;
}

} // namespace

namespace sprint_timer::api {

TaskTreeDTO makeDTO(const TaskTree& taskTree)
{
    auto mapNode = [](const TaskNode& taskNode) {
        return TaskNodeDTO{makeDTO(taskNode.task),
                           makeTaskTypeDTO(taskNode.type),
                           taskNode.dueTime,
                           taskNode.reminder,
                           taskNode.notes};
    };
    return taskTree.mapped<TaskNodeDTO>(mapNode);
}

TaskTree fromDTO(const TaskTreeDTO& taskTreeDto)
{
    auto mapNode = [](const TaskNodeDTO& taskNode) {
        return TaskNode{fromDTO(taskNode.task),
                        fromTaskTypeDTO(taskNode.type),
                        taskNode.dueTime,
                        taskNode.reminder,
                        taskNode.notes};
    };
    return taskTreeDto.mapped<TaskNode>(mapNode);
}

} // namespace sprint_timer::api
