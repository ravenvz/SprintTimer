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
#ifndef TASKMAPPER_H_92B3YBWU
#define TASKMAPPER_H_92B3YBWU

#include "api/dtos/TaskDTO.h"
#include "core/Task.h"
#include <ranges>
#include <span>

namespace sprint_timer::api {

TaskDTO makeDTO(const sprint_timer::Task& task);

Task fromDTO(const TaskDTO& dto);

inline auto dtoAdapter(std::span<const Task> tasks)
{
    return std::views::transform(
        tasks, [](const auto& task) { return makeDTO(task); });
}

inline auto dtoAdapter(std::span<const TaskDTO> dtos)
{
    return std::views::transform(dtos,
                                 [](const auto& dto) { return fromDTO(dto); });
}

} // namespace sprint_timer::api

#endif /* end of include guard: TASKMAPPER_H_92B3YBWU */
