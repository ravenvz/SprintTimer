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
#include "core/entities/Sprint.h"
#include "core/StringUtils.h"
#include <algorithm>


// static
BoostUUIDGenerator Sprint::generator;

Sprint::Sprint(const std::string& taskName,
               const TimeSpan& timeSpan,
               const std::list<Tag>& tags,
               const std::string& uuid,
               const std::string& taskUuid)
    : mName{taskName}
    , mInterval{timeSpan}
    , mUuid{uuid}
    , mTaskUuid{taskUuid}
    , mTags{tags}
{
}

Sprint::Sprint(const std::string& taskName,
               const TimeSpan& timeSpan,
               const std::list<Tag>& tags,
               const std::string& taskUuid)
    : Sprint{taskName, timeSpan, tags, generator.generateUUID(), taskUuid}
{
}

// TODO What's up with name? What's up with tags?
Sprint::Sprint(const std::string& taskUuid, const TimeSpan& timeSpan)
    : mInterval{timeSpan}
    , mUuid{generator.generateUUID()}
    , mTaskUuid{taskUuid}
{
}

std::string Sprint::name() const { return mName; }

DateTime Sprint::startTime() const { return mInterval.start(); }

DateTime Sprint::finishTime() const { return mInterval.finish(); }

TimeSpan Sprint::timeSpan() const { return mInterval; }

std::string Sprint::uuid() const { return mUuid; }

std::string Sprint::taskUuid() const { return mTaskUuid; }

std::list<Tag> Sprint::tags() const { return mTags; }

std::ostream& operator<<(std::ostream& os, const Sprint& sprint)
{
    for (const auto& tag : sprint.mTags)
        os << tag.nameWithPrefix();
    os << " " << sprint.mName << " " << sprint.mInterval << " Uuid: " << sprint.mUuid
       << " TaskUuid: " << sprint.mTaskUuid;
    return os;
}

