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
#include "api/dtos/TaskTimeframeMapper.h"
#include "core/Recurrence.h"

namespace sprint_timer::api {

auto TaskTimeframeMapper::make_dto_impl(const TaskTimeframe& frame) const
    -> TaskTimeframeDTO
{
    const auto recurrence = frame.recurrence.transform(
        [](const auto& recurr) { return recurr.pattern(); });
    return TaskTimeframeDTO{frame.start, frame.due, frame.remindAt, recurrence};
}

auto TaskTimeframeMapper::make_entity_impl(const TaskTimeframeDTO& dto) const
    -> TaskTimeframe
{
    const auto recurrence = dto.recurrence.transform(
        [](const auto& pattern) { return Recurrence{pattern}; });
    return TaskTimeframe{dto.start, dto.due, dto.remindAt, recurrence};
}

} // namespace sprint_timer::api
