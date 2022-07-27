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
#ifndef SPRINT_H_XWNOT32M
#define SPRINT_H_XWNOT32M

#include "core/Tag.h"
#include "date_wrapper/date_wrapper.h"
#include <ostream>
#include <string>

namespace sprint_timer {

class Sprint {
public:
    Sprint(dw::DateTimeRange timeSpan_)
        : startTime{timeSpan_.start()}
        , length{timeSpan_.duration<std::chrono::minutes>()}
    {
    }

    Sprint(dw::DateTime start_, std::chrono::minutes duration)
        : startTime{start_}
        , length{duration}
    {
    }

    dw::DateTime start() const { return startTime; }

    dw::DateTime finish() const { return startTime + length; }

    std::chrono::minutes duration() const { return length; }

    dw::DateTimeRange timeSpan() const
    {
        return dw::DateTimeRange{startTime, startTime + length};
    }

private:
    dw::DateTime startTime;
    std::chrono::minutes length;
};

bool operator==(const Sprint& lhs, const Sprint& rhs);

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const Sprint& sprint)
{
    os << "Sprint{" << sprint.timeSpan() << "}";
    return os;
}

/* Represents Sprint concept.
 *
 * Sprint is an n-minute time interval that is associated with
 * a Task. It has the same name and tags it's associated Task has.
 */
class SprintRecord {

public:
    SprintRecord(std::string taskName,
                 dw::DateTimeRange timeSpan,
                 std::vector<Tag> tags);

    // Sprint name is identical to it's associated task name.
    std::string taskName() const;

    dw::DateTime startTime() const;

    dw::DateTime finishTime() const;

    dw::DateTimeRange timeSpan() const;

    /* Tags are identical the associated task tags.
     * Order of tags in the list is not specified. */
    std::vector<Tag> tags() const;

private:
    std::string name_;
    dw::DateTimeRange timeSpan_;
    std::vector<Tag> tags_;
};

std::ostream& operator<<(std::ostream& os, const SprintRecord& sprint);

bool operator==(const SprintRecord& lhs, const SprintRecord& rhs);

bool intersectingInTime(const SprintRecord& lhs, const SprintRecord& rhs);

bool intersectingInTime(const Sprint& lhs, const Sprint& rhs);

} // namespace sprint_timer

#endif // SPRINT_H_XWNOT32M
