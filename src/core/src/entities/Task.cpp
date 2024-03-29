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
#include "core/entities/Task.h"
#include "core/BoostUUIDGenerator.h"
#include <iostream>

namespace sprint_timer::entities {

using dw::DateTime;

Task::Task(std::string name,
           int estimatedCost,
           int actualCost,
           std::string uuid,
           std::list<Tag> tags,
           bool completed,
           const DateTime& lastModified)
    : name_{std::move(name)}
    , estimatedCost_{estimatedCost}
    , actualCost_{actualCost}
    , uuid_{uuid}
    , tags_{std::move(tags)}
    , completed_{completed}
    , lastModified_{lastModified}
{
}

std::string Task::name() const { return name_; }

bool Task::isCompleted() const { return completed_; }

int Task::estimatedCost() const { return estimatedCost_; }

int Task::actualCost() const { return actualCost_; }

std::string Task::uuid() const { return uuid_; }

std::list<Tag> Task::tags() const { return tags_; }

DateTime Task::lastModified() const { return lastModified_; }

void Task::setName(const std::string& name) { name_ = name; }

void Task::setCompleted(bool completed) { completed_ = completed; }

void Task::setEstimatedCost(int numSprints) { estimatedCost_ = numSprints; }

void Task::setTags(const std::list<Tag>& newTags) { tags_ = newTags; }

void Task::setActualCost(int numSprints) { actualCost_ = numSprints; }

void Task::setModifiedTimeStamp(const DateTime& timeStamp)
{
    lastModified_ = timeStamp;
}

std::ostream& operator<<(std::ostream& os, const Task& task)
{
    os << prefixTags(task.tags());
    if (!task.tags().empty())
        os << " ";
    os << task.name() << " ";
    os << task.actualCost() << "/" << task.estimatedCost() << " ";
    os << "Uuid: " << task.uuid() << " ";
    os << task.lastModified();
    return os;
}

bool operator==(const Task& lhs, const Task& rhs)
{
    return lhs.uuid() == rhs.uuid() && lhs.name() == rhs.name() &&
           lhs.estimatedCost() == rhs.estimatedCost() &&
           lhs.actualCost() == rhs.actualCost() && lhs.tags() == rhs.tags() &&
           lhs.isCompleted() == rhs.isCompleted()
           // There is a reason to compare them by seconds, as last modified
           // timestamp can come from different sources with different precision
           // TODO need to control the sources as this can end up badly
           && dw::to_time_point<std::chrono::seconds>(lhs.lastModified()) ==
                  dw::to_time_point<std::chrono::seconds>(rhs.lastModified());
}

} // namespace sprint_timer::entities
