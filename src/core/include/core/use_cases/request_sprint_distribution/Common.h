/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#ifndef COMMON_H_U7S8D6JF
#define COMMON_H_U7S8D6JF

#include "core/entities/Sprint.h"

namespace sprint_timer::use_cases {

std::vector<sprint_timer::entities::Sprint>
filterByTag(const std::vector<sprint_timer::entities::Sprint>& sprints,
            const sprint_timer::entities::Tag& tag);

constexpr bool containsDate(const dw::DateRange& dateRange,
                            const dw::Date& date) noexcept
{
    return dateRange.start() <= date && date <= dateRange.finish();
}

constexpr size_t sizeInDays(const dw::DateRange& dateRange) noexcept
{
    return static_cast<size_t>(dateRange.duration().count()) + 1;
}

} // namespace sprint_timer::use_cases

#endif /* end of include guard: COMMON_H_U7S8D6JF */
