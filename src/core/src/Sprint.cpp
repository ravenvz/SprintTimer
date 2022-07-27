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
#include "core/Sprint.h"
#include "core/utils/StringUtils.h"
#include <algorithm>
#include <utility>

namespace {

bool areConsecutive(const dw::DateTimeRange& lhs, const dw::DateTimeRange& rhs);

bool inRange(dw::DateTime dateTime, dw::DateTimeRange range);

bool equalToMinutes(const dw::DateTime& lhs, const dw::DateTime& rhs);

} // namespace

namespace sprint_timer {

using dw::DateTime;
using dw::DateTimeRange;

bool operator==(const Sprint& lhs, const Sprint& rhs)
{
    return rhs.duration() == rhs.duration() &&
           // There is a reason to compare DateTimeRanges by seconds
           // TODO need to control the sources, see also todo at Task
           dw::to_time_point<std::chrono::seconds>(lhs.start()) ==
               dw::to_time_point<std::chrono::seconds>(rhs.start());
}

bool intersectingInTime(const Sprint& lhs, const Sprint& rhs)
{
    if (areConsecutive(lhs.timeSpan(), rhs.timeSpan())) {
        return false;
    }
    return inRange(lhs.timeSpan().start(), rhs.timeSpan()) ||
           inRange(lhs.timeSpan().finish(), rhs.timeSpan());
}

} // namespace sprint_timer

namespace {

bool areConsecutive(const dw::DateTimeRange& lhs, const dw::DateTimeRange& rhs)
{
    return equalToMinutes(lhs.start(), rhs.finish()) ||
           equalToMinutes(lhs.finish(), rhs.start());
}

bool inRange(dw::DateTime dateTime, dw::DateTimeRange range)
{
    return range.start() <= dateTime && dateTime <= range.finish();
}

bool equalToMinutes(const dw::DateTime& lhs, const dw::DateTime& rhs)
{
    return lhs.date() == rhs.date() && lhs.hour() == rhs.hour() &&
           lhs.minute() == rhs.minute();
}

} // namespace
