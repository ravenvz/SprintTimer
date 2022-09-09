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
#include "api/dtos/TaskTypeMapper.h"

namespace sprint_timer::api {

auto makeDTO(TaskType taskType) -> TaskTypeDTO
{
    using enum TaskType;

    switch (taskType) {
    case Project:
        return TaskTypeDTO::Project;
    case Folder:
        return TaskTypeDTO::Folder;
    case Regular:
        return TaskTypeDTO::Regular;
    };

    return TaskTypeDTO::Regular;
}

auto fromDTO(const TaskTypeDTO& taskType) -> TaskType
{
    using enum TaskTypeDTO;

    switch (taskType) {
    case Project:
        return TaskType::Project;
        break;
    case Folder:
        return TaskType::Folder;
    case Regular:
        return TaskType::Regular;
    };

    return TaskType::Regular;
}

} // namespace sprint_timer::api
