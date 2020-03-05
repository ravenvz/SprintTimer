/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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

#include "core/SprintBuilder.h"

namespace sprint_timer {

using namespace entities;

Sprint SprintBuilder::build()
{
    using namespace std::chrono;
    if (!taskUuid_)
        throw SprintBuilderError{"Can't build Sprint without Task uuid"};
    if (!timeSpan_)
        throw SprintBuilderError{"Can't build Sprint without timespan"};
    if (!uuid_)
        return Sprint{name_, *timeSpan_, tags_, *taskUuid_};
    Sprint sprint{name_, *timeSpan_, tags_, *uuid_, *taskUuid_};
    // Clean uuid, so that a new one can be generated for next built sprint
    uuid_ = std::nullopt;
    return sprint;
}

SprintBuilder& SprintBuilder::withName(std::string name)
{
    name_ = std::move(name);
    return *this;
}

SprintBuilder& SprintBuilder::withUuid(std::string uuid)
{
    uuid_ = std::make_optional(std::move(uuid));
    return *this;
}

SprintBuilder& SprintBuilder::withTaskUuid(std::string uuid)
{
    taskUuid_ = std::make_optional(std::move(uuid));
    return *this;
}

SprintBuilder& SprintBuilder::withTag(Tag tag)
{
    tags_.push_back(std::move(tag));
    return *this;
}

SprintBuilder& SprintBuilder::withTag(std::string tag)
{
    tags_.push_back(Tag{std::move(tag)});
    return *this;
}

SprintBuilder& SprintBuilder::withTimeSpan(dw::DateTimeRange timeSpan)
{
    timeSpan_ = std::make_optional(std::move(timeSpan));
    return *this;
}

SprintBuilder& SprintBuilder::forTask(const Task& task)
{
    name_ = task.name();
    taskUuid_ = std::make_optional(task.uuid());
    tags_ = task.tags();
    return *this;
}

SprintBuilder& SprintBuilder::withExplicitTags(std::list<Tag> tags)
{
    tags_ = std::move(tags);
    return *this;
}

} // namespace sprint_timer
