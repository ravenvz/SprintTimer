/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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

std::string TimeSpan::toString(const std::string& format, std::string sep) const
{
    std::stringstream ss;
    std::string separator{std::move(sep)};
    ss << startTime.toString(format) << separator
       << finishTime.toString(format);
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const TimeSpan& span)
{
    os << "TimeSpan {" << span.startTime << ", " << span.finishTime << "}";
    return os;
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
