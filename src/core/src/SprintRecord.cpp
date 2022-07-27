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
#include "core/SprintRecord.h"

using dw::DateTime;
using dw::DateTimeRange;
using sprint_timer::Tag;

namespace sprint_timer {

SprintRecord::SprintRecord(std::string taskName,
                           DateTimeRange timeSpan,
                           std::vector<Tag> tags)
    : name_{std::move(taskName)}
    , timeSpan_{timeSpan}
    , tags_{std::move(tags)}
{
}

std::string SprintRecord::taskName() const { return name_; }

DateTime SprintRecord::startTime() const { return timeSpan_.start(); }

DateTime SprintRecord::finishTime() const { return timeSpan_.finish(); }

DateTimeRange SprintRecord::timeSpan() const { return timeSpan_; }

std::vector<Tag> SprintRecord::tags() const { return tags_; }

std::ostream& operator<<(std::ostream& os, const SprintRecord& sprint)
{
    for (const auto& tag : sprint.tags())
        os << tag.nameWithPrefix();
    os << " " << sprint.taskName() << " " << sprint.timeSpan();
    return os;
}

bool operator==(const SprintRecord& lhs, const SprintRecord& rhs)
{
    return lhs.taskName() == rhs.taskName()
           // There is a reason to compare DateTimeRanges by seconds
           // TODO need to control the sources, see also todo at Task
           && dw::to_time_point<std::chrono::seconds>(lhs.timeSpan().start()) ==
                  dw::to_time_point<std::chrono::seconds>(
                      rhs.timeSpan().start()) &&
           dw::to_time_point<std::chrono::seconds>(lhs.timeSpan().finish()) ==
               dw::to_time_point<std::chrono::seconds>(
                   rhs.timeSpan().finish()) &&
           lhs.tags() == rhs.tags();
}

// bool intersectingInTime(const SprintRecord& lhs, const SprintRecord& rhs)
// {
//     if (areConsecutive(lhs.timeSpan(), rhs.timeSpan())) {
//         return false;
//     }
//
//     return inRange(lhs.startTime(), rhs.timeSpan()) ||
//            inRange(lhs.finishTime(), rhs.timeSpan());
// }

} // namespace sprint_timer
