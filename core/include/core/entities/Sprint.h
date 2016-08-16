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
#ifndef POMODORO_POMODORO_H
#define POMODORO_POMODORO_H


#include "core/BoostUUIDGenerator.h"
#include "core/TimeSpan.h"
#include "core/entities/Tag.h"
#include "core/entities/Task.h"
#include <list>
#include <string>

/* Represents Sprint concept.
 *
 * Sprint is an n-minute time interval that is associated with
 * a Task. It has same name and tags it's associated Task has.
 */
class Sprint {

public:

    Sprint(const std::string& taskName,
           const TimeSpan& timeSpan,
           const std::list<Tag>& tags,
           const std::string& uuid,
           const std::string& taskUuid);

    Sprint(const std::string& taskUuid, const TimeSpan& timeSpan);

    // Return name of Sprint. It is identical to the associated task name.
    std::string name() const;

    // Return starting date and time.
    DateTime startTime() const;

    // Return finishing date and time.
    DateTime finishTime() const;

    // Return time interval when this Sprint was active.
    TimeSpan timeSpan() const;

    // Return uuid.
    std::string uuid() const;

    // Return associated Task uuid.
    std::string taskUuid() const;

    // Return vector of tags. Tags are identical the associated task tags.
    std::list<Tag> tags() const;

    // Return string representation.
    std::string toString() const;

private:
    static BoostUUIDGenerator generator;
    std::string mName;
    TimeSpan mInterval;
    std::string mUuid;
    std::string mTaskUuid;
    std::list<Tag> mTags;
};


#endif // POMODORO_POMODORO_H
