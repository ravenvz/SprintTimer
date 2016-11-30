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

#include "core/TaskBuilder.h"

Task TaskBuilder::build()
{
    if (!lastModified)
        lastModified = make_optional(DateTime::currentDateTimeLocal());
    if (!uuid)
        return Task{name, estimatedCost, actualCost, tags, completionStatus, *lastModified};
    Task task{name, estimatedCost, actualCost, *uuid, tags, completionStatus, *lastModified};
    uuid = optional<std::string>{};
    lastModified = optional<DateTime>{};
    return task;
}

TaskBuilder& TaskBuilder::withName(std::string name)
{
    this->name = std::move(name);
    return *this;
}

TaskBuilder& TaskBuilder::withEstimatedCost(int cost)
{
    estimatedCost = cost;
    return *this;
}

TaskBuilder& TaskBuilder::withActualCost(int cost)
{
    actualCost = cost;
    return *this;
}

TaskBuilder& TaskBuilder::withUuid(std::string uuid)
{
    this->uuid = make_optional(std::move(uuid));
    return *this;
}

TaskBuilder& TaskBuilder::withTag(std::string tag)
{
    tags.emplace_back(Tag{std::move(tag)});
    return *this;
}

TaskBuilder& TaskBuilder::withTag(Tag tag)
{
    tags.emplace_back(std::move(tag));
    return *this;
}

TaskBuilder& TaskBuilder::withExplicitTags(std::list<Tag> tags)
{
    this->tags = std::move(tags);
    return *this;
}

TaskBuilder& TaskBuilder::withLastModificationStamp(DateTime timeStamp)
{
    lastModified = make_optional(std::move(timeStamp));
    return *this;
}

TaskBuilder& TaskBuilder::withCompletionStatus(bool completed)
{
    completionStatus = completed;
    return *this;
}
