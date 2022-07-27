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
#ifndef SPRINTMAPPER_H_U4OQ1PMS
#define SPRINTMAPPER_H_U4OQ1PMS

#include "api/dtos/SprintDTO.h"
#include "core/Sprint.h"
#include <ranges>
#include <span>

namespace sprint_timer::api {

auto makeDTO(const Sprint& sprint) -> dw::DateTimeRange;

auto fromDTO(const dw::DateTimeRange& dto) -> Sprint;

inline auto dtoAdapter(std::span<const Sprint> sprints)
{
    return std::views::transform(
        sprints, [](const auto& sprint) { return makeDTO(sprint); });
}

inline auto dtoAdapter(std::span<const dw::DateTimeRange> dtos)
{
    return std::views::transform(dtos,
                                 [](const auto& dto) { return fromDTO(dto); });
}

auto fromDTO(const SprintDTO& dto) -> SprintRecord;

auto makeDTO(const SprintRecord& sprint) -> SprintDTO;

inline auto dtoAdapter(std::span<const SprintRecord> sprints)
{
    return std::views::transform(
        sprints, [](const auto& sprint) { return makeDTO(sprint); });
}

inline auto dtoAdapter(std::span<const SprintDTO> dtos)
{
    return std::views::transform(dtos,
                                 [](const auto& dto) { return fromDTO(dto); });
}

} // namespace sprint_timer::api

#endif /* end of include guard: SPRINTMAPPER_H_U4OQ1PMS */
