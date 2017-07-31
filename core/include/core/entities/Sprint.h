/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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

#include "core/BoostUUIDGenerator.h"
#include "core/entities/Tag.h"
#include "core/entities/Task.h"
#include "date_wrapper/TimeSpan.h"
#include <list>
#include <ostream>
#include <string>

/* Represents Sprint concept.
 *
 * Sprint is an n-minute time interval that is associated with
 * a Task. It has the same name and tags it's associated Task has.
 */
class Sprint {

public:
    Sprint(const std::string& taskName,
           const dw::TimeSpan& timeSpan,
           const std::list<Tag>& tags,
           const std::string& uuid,
           const std::string& taskUuid);

    Sprint(const std::string& taskName,
           const dw::TimeSpan& timeSpan,
           const std::list<Tag>& tag,
           const std::string& taskUuid);

    Sprint(const std::string& taskUuid, const dw::TimeSpan& timeSpan);

    // Return name of Sprint. It is identical to the associated task name.
    std::string name() const;

    // Return starting date and time.
    DateTime startTime() const;

    // Return finishing date and time.
    DateTime finishTime() const;

    // Return time interval when this Sprint was active.
    dw::TimeSpan timeSpan() const;

    // Return uuid.
    std::string uuid() const;

    // Return associated Task uuid.
    std::string taskUuid() const;

    /* Return vector of tags. Tags are identical the associated task tags.
     * Order of tags in the list is not specified.
     */
    std::list<Tag> tags() const;

    friend std::ostream& operator<<(std::ostream& os, const Sprint& sprint);

private:
    static BoostUUIDGenerator generator;
    std::string mName;
    dw::TimeSpan mInterval;
    std::string mUuid;
    std::string mTaskUuid;
    std::list<Tag> mTags;
};

#endif // SPRINT_H_XWNOT32M
