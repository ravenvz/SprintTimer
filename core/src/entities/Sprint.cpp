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
#include "core/entities/Sprint.h"
#include "core/BoostUUIDGenerator.h"
#include "core/utils/StringUtils.h"
#include <algorithm>

namespace {

sprint_timer::BoostUUIDGenerator uuid_generator;

}

namespace sprint_timer::entities {

using dw::DateTime;
using dw::DateTimeRange;

Sprint::Sprint(const std::string& taskName,
               const DateTimeRange& timeSpan,
               const std::list<Tag>& tags,
               const std::string& uuid,
               const std::string& taskUuid)
    : name_{taskName}
    , timeSpan_{timeSpan}
    , uuid_{uuid}
    , taskUuid_{taskUuid}
    , tags_{tags}
{
}

Sprint::Sprint(const std::string& taskName,
               const DateTimeRange& timeSpan,
               const std::list<Tag>& tags,
               const std::string& taskUuid)
    : Sprint{taskName, timeSpan, tags, uuid_generator.generateUUID(), taskUuid}
{
}

// TODO What's up with name? What's up with tags?
Sprint::Sprint(const std::string& taskUuid, const DateTimeRange& timeSpan)
    : timeSpan_{timeSpan}
    , uuid_{uuid_generator.generateUUID()}
    , taskUuid_{taskUuid}
{
}

std::string Sprint::name() const { return name_; }

DateTime Sprint::startTime() const { return timeSpan_.start(); }

DateTime Sprint::finishTime() const { return timeSpan_.finish(); }

DateTimeRange Sprint::timeSpan() const { return timeSpan_; }

std::string Sprint::uuid() const { return uuid_; }

std::string Sprint::taskUuid() const { return taskUuid_; }

std::list<Tag> Sprint::tags() const { return tags_; }

std::ostream& operator<<(std::ostream& os, const Sprint& sprint)
{
    for (const auto& tag : sprint.tags())
        os << tag.nameWithPrefix();
    os << " " << sprint.name() << " " << sprint.timeSpan()
       << " Uuid: " << sprint.uuid() << " TaskUuid: " << sprint.taskUuid();
    return os;
}

bool operator==(const Sprint& lhs, const Sprint& rhs)
{
    return lhs.taskUuid() == rhs.taskUuid() && lhs.uuid() == rhs.uuid()
        && lhs.name() == rhs.name()
        // There is a reason to compare DateTimeRanges by seconds
        // TODO need to control the sources, see also todo at Task
        && dw::to_time_point<std::chrono::seconds>(lhs.timeSpan().start())
        == dw::to_time_point<std::chrono::seconds>(rhs.timeSpan().start())
        && dw::to_time_point<std::chrono::seconds>(lhs.timeSpan().finish())
        == dw::to_time_point<std::chrono::seconds>(rhs.timeSpan().finish())
        && lhs.tags() == rhs.tags();
}

} // namespace sprint_timer::entities
