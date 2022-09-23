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

auto TaskTypeMapper::convert(const TaskType& kind) const -> TaskTypeDTO
{
    using enum TaskType;

    switch (kind) {
    case Project:
        return TaskTypeDTO::Project;
    case Folder:
        return TaskTypeDTO::Folder;
    case Regular:
        return TaskTypeDTO::Regular;
    default:
        throw std::runtime_error{"TaskTypeMapper: wrong enum type"};
    }
}

auto TaskTypeMapper::convert(const TaskTypeDTO& dto) const -> TaskType
{

    using enum TaskTypeDTO;

    switch (dto) {
    case Project:
        return TaskType::Project;
    case Folder:
        return TaskType::Folder;
    case Regular:
        return TaskType::Regular;
    default:
        throw std::runtime_error{"TaskTypeMapper: wrong enum type"};
    }
}

} // namespace sprint_timer::api
