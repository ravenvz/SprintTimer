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
#include "core/utils/Algutils.h"

namespace sprint_timer::api {

auto TaskTimeframeMapper::convert(const TaskTimeframe& frame) const
    -> TaskTimeframeDTO
{
    const auto recurrence = utils::transform(
        frame.recurrence, [](const auto& recurr) { return recurr.pattern(); });
    return TaskTimeframeDTO{frame.start, frame.due, frame.remindAt, recurrence};
}

auto TaskTimeframeMapper::convert(const TaskTimeframeDTO& dto) const
    -> TaskTimeframe
{
    const auto recurrence =
        utils::transform(dto.recurrence, [](const auto& pattern) {
            return Recurrence{pattern};
        });
    return TaskTimeframe{dto.start, dto.due, dto.remindAt, recurrence};
}

} // namespace sprint_timer::api
