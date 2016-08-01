/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "core/TimeSpan.h"


TimeSpan::TimeSpan(SystemClock start, SystemClock finish)
    : startTime{DateTime{std::move(start)}}
    , finishTime{DateTime{std::move(finish)}}
{
}

TimeSpan::TimeSpan(const DateTime& start, const DateTime& finish)
    : startTime{start}
    , finishTime{finish}
{
}

TimeSpan::TimeSpan(std::time_t start,
                   std::time_t finish,
                   int offsetFromUtcInSeconds)
    : startTime{DateTime::fromTime_t(start, offsetFromUtcInSeconds)}
    , finishTime{DateTime::fromTime_t(finish, offsetFromUtcInSeconds)}
{
}

unsigned TimeSpan::sizeInDays() const
{
    if (startTime <= finishTime) {
        return static_cast<unsigned>(startTime.daysTo(finishTime) + 1);
    }
    else {
        return static_cast<unsigned>(finishTime.daysTo(startTime) + 1);
    }
}

TimeSpan::DayPart TimeSpan::getDayPart() const
{
    auto hour = startTime.hour();

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

/* static */
std::string TimeSpan::dayPartName(unsigned dayPart)
{
    return dayPartName(static_cast<DayPart>(dayPart));
}

/* static */
std::string TimeSpan::dayPartName(DayPart dayPart)
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

/* static */
std::string TimeSpan::dayPartHours(unsigned dayPart)
{
    return dayPartHours(static_cast<DayPart>(dayPart));
}

/* static */
std::string TimeSpan::dayPartHours(DayPart dayPart)
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

std::string TimeSpan::toTimeString() const
{
    std::string res{startTime.toTimeString()};
    res += " - ";
    res += finishTime.toTimeString();
    return res;
}

std::string TimeSpan::toDateString() const
{
    std::string res;
    res += startTime.ddmmyyyyString();
    res += " - ";
    res += finishTime.ddmmyyyyString();
    return res;
}

unsigned startDateAbsDiff(const TimeSpan& one, const TimeSpan& other)
{
    if (one.startTime <= other.startTime) {
        return static_cast<unsigned>(one.startTime.daysTo(other.startTime));
    }
    else {
        return static_cast<unsigned>(other.startTime.daysTo(one.startTime));
    }
}
