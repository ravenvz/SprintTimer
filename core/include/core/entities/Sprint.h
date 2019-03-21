/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

#include "core/entities/Tag.h"
#include "date_wrapper/date_wrapper.h"
#include <list>
#include <ostream>
#include <string>

namespace sprint_timer::entities {

/* Represents Sprint concept.
 *
 * Sprint is an n-minute time interval that is associated with
 * a Task. It has the same name and tags it's associated Task has.
 */
class Sprint {

public:
    Sprint(const std::string& taskName,
           const dw::DateTimeRange& timeSpan,
           const std::list<Tag>& tags,
           const std::string& uuid,
           const std::string& taskUuid);

    Sprint(const std::string& taskName,
           const dw::DateTimeRange& timeSpan,
           const std::list<Tag>& tags,
           const std::string& taskUuid);

    Sprint(const std::string& taskUuid, const dw::DateTimeRange& timeSpan);

    // Sprint name is identical to it's associated task name.
    std::string name() const;

    dw::DateTime startTime() const;

    dw::DateTime finishTime() const;

    dw::DateTimeRange timeSpan() const;

    std::string uuid() const;

    std::string taskUuid() const;

    /* Tags are identical the associated task tags.
     * Order of tags in the list is not specified. */
    std::list<Tag> tags() const;

private:
    std::string name_;
    dw::DateTimeRange timeSpan_;
    std::string uuid_;
    std::string taskUuid_;
    std::list<Tag> tags_;
};

std::ostream& operator<<(std::ostream& os, const Sprint& sprint);

bool operator==(const Sprint& lhs, const Sprint& rhs);

} // namespace sprint_timer::entities

#endif // SPRINT_H_XWNOT32M
