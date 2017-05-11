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

#include "core/SprintBuilder.h"

Sprint SprintBuilder::build()
{
    using namespace std::chrono;
    if (!taskUuid)
        throw SprintBuilderError{"Can't build Sprint without Task uuid"};
    // TODO remove magic constant. It should be taken from config anyway
    if (!timeSpan)
        throw SprintBuilderError{"Can't build Sprint without timespan"};
    if (!uuid)
        return Sprint{name, *timeSpan, tags, *taskUuid};
    Sprint sprint{name, *timeSpan, tags, *uuid, *taskUuid};
    uuid = optional<std::string>{};
    return sprint;
}

SprintBuilder& SprintBuilder::withName(std::string name)
{
    this->name = std::move(name);
    return *this;
}

SprintBuilder& SprintBuilder::withUuid(std::string uuid)
{
    this->uuid = make_optional(std::move(uuid));
    return *this;
}

SprintBuilder& SprintBuilder::withTaskUuid(std::string uuid)
{
    this->taskUuid = make_optional(std::move(uuid));
    return *this;
}

SprintBuilder& SprintBuilder::withTag(Tag tag)
{
    tags.emplace_back(std::move(tag));
    return *this;
}

SprintBuilder& SprintBuilder::withTag(std::string tag)
{
    tags.emplace_back(Tag{std::move(tag)});
    return *this;
}

SprintBuilder& SprintBuilder::withTimeSpan(TimeSpan timeSpan)
{
    this->timeSpan = make_optional(std::move(timeSpan));
    return *this;
}

SprintBuilder& SprintBuilder::forTask(const Task& task)
{
    name = std::move(task.name());
    taskUuid = make_optional(task.uuid());
    tags = std::move(task.tags());
    return *this;
}

SprintBuilder& SprintBuilder::withExplicitTags(std::list<Tag> tags)
{
    this->tags = std::move(tags);
    return *this;
}

