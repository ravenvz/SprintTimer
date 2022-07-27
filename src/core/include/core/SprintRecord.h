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
#ifndef SPRINTRECORD_H_L0VWMOSF
#define SPRINTRECORD_H_L0VWMOSF

#include "core/Tag.h"
#include "date_wrapper/date_wrapper.h"
#include <vector>

namespace sprint_timer {

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

// bool intersectingInTime(const SprintRecord& lhs, const SprintRecord& rhs);

} // namespace sprint_timer

#endif /* end of include guard: SPRINTRECORD_H_L0VWMOSF */
