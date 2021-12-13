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
#include "core/use_cases/request_sprint_distribution/DayPart.h"
#include <array>
#include <string_view>

namespace {

constexpr size_t numDayParts{6};

constexpr std::array<std::string_view, numDayParts> dayPartNameTable{
    "Midnight", "Night", "Morning", "Noon", "Afternoon", "Evening"};

constexpr std::array<std::string_view, numDayParts> dayPartHoursTable{
    "22:00 - 2:00",
    "2:00 - 6:00",
    "6:00 - 10:00",
    "10:00 - 14:00",
    "14:00 - 18:00",
    "18:00 - 22:00"};

constexpr std::string_view invalid{"Invalid"};

} // namespace

namespace sprint_timer::use_cases {

DayPart dayPart(const dw::DateTimeRange& timeSpan)
{
    const auto hour = timeSpan.start().hour().count();
    using enum DayPart;

    if (22 < hour || hour <= 2) {
        return Midnight;
    }
    else if (2 < hour && hour <= 6) {
        return Night;
    }
    else if (6 < hour && hour <= 10) {
        return Morning;
    }
    else if (10 < hour && hour <= 14) {
        return Noon;
    }
    else if (14 < hour && hour <= 18) {
        return Afternoon;
    }
    else {
        return Evening;
    }
}

std::string dayPartName(unsigned dayPart)
{
    return dayPartName(static_cast<DayPart>(dayPart));
}

std::string dayPartName(DayPart dayPart)
{
    const auto num = static_cast<size_t>(dayPart);
    return std::string{num < numDayParts ? dayPartNameTable[num] : invalid};
}

std::string dayPartHours(DayPart dayPart)
{
    const auto num = static_cast<size_t>(dayPart);
    return std::string{num < numDayParts ? dayPartHoursTable[num] : invalid};
}

std::string dayPartHours(unsigned dayPart)
{
    return dayPartHours(static_cast<DayPart>(dayPart));
}

} // namespace sprint_timer::use_cases

