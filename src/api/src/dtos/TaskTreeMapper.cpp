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

namespace sprint_timer::api {

TaskTreeMapper::TaskTreeMapper(const Converter<TaskDTO, Task>& taskMapper_)
    : taskMapper{taskMapper_}
{
}

auto TaskTreeMapper::convert(const TaskTree& tree) const -> TaskTreeDTO
{
    auto mapNode = [&](const auto& task) { return taskMapper(task); };
    return tree.mapped<TaskDTO>(mapNode);
}

auto TaskTreeMapper::convert(const TaskTreeDTO& dto) const -> TaskTree
{
    auto mapNode = [&](const auto& taskDto) { return taskMapper(taskDto); };
    return dto.mapped<Task>(mapNode);
}

} // namespace sprint_timer::api
