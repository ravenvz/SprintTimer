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
#ifndef DAYPART_H_X1VWLXSB
#define DAYPART_H_X1VWLXSB

#include <date_wrapper/date_wrapper.h>
#include <string>

namespace sprint_timer::use_cases {

constexpr size_t numParts{6};

/* Represent a day partition
 *
 * Day has 6 4-hour parts:
 *      Midnight  22:00 - 2:00
 *      Night      2:00 - 6:00
 *      Morning    6:00 - 10:00
 *      Noon      10:00 - 14:00
 *      Afternoon 14:00 - 18:00
 *      Evening   18:00 - 22:00
 */
enum class DayPart { Midnight, Night, Morning, Noon, Afternoon, Evening };

std::string dayPartHours(unsigned dayPart);

DayPart dayPart(const dw::DateTimeRange& timeSpan);

std::string dayPartName(unsigned dayPart);

std::string dayPartName(DayPart dayPart);

std::string dayPartHours(DayPart dayPart);

std::string dayPartHours(unsigned dayPart);

DayPart dayPart(const DateTimeRange& timeSpan)
{
    const auto hour = timeSpan.start().hour().count();

    if (22 < hour || hour <= 2) {
        return DayPart::Midnight;
    }
    else if (2 < hour && hour <= 6) {
        return DayPart::Night;
    }
    else if (6 < hour && hour <= 10) {
        return DayPart::Morning;
    }
    else if (10 < hour && hour <= 14) {
        return DayPart::Noon;
    }
    else if (14 < hour && hour <= 18) {
        return DayPart::Afternoon;
    }
    else {
        return DayPart::Evening;
    }
}

std::string dayPartName(unsigned dayPart)
{
    return dayPartName(static_cast<DayPart>(dayPart));
}

std::string dayPartName(DayPart dayPart)
{
    switch (dayPart) {
    case DayPart::Midnight:
        return "Midnight";
    case DayPart::Night:
        return "Night";
    case DayPart::Morning:
        return "Morning";
    case DayPart::Noon:
        return "Noon";
    case DayPart::Afternoon:
        return "Afternoon";
    case DayPart::Evening:
        return "Evening";
    }
    return "Invalid";
}

std::string dayPartHours(DayPart dayPart)
{
    switch (dayPart) {
    case DayPart::Midnight:
        return "22:00 - 2:00";
    case DayPart::Night:
        return "2:00 - 6:00";
    case DayPart::Morning:
        return "6:00 - 10:00";
    case DayPart::Noon:
        return "10:00 - 14:00";
    case DayPart::Afternoon:
        return "14:00 - 18:00";
    case DayPart::Evening:
        return "18:00 - 22:00";
    }
    return "Invalid";
}

std::string dayPartHours(unsigned dayPart)
{
    return dayPartHours(static_cast<DayPart>(dayPart));
}

} // namespace sprint_timer::use_cases

#endif /* end of include guard: DAYPART_H_X1VWLXSB */
