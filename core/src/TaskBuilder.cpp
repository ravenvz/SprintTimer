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

#include "core/TaskBuilder.h"

namespace sprint_timer {

using dw::DateTime;
using namespace entities;

Task TaskBuilder::build()
{
    if (!mLastModified)
        mLastModified = std::make_optional(dw::current_date_time_local());
    if (!mUuid)
        return Task{mName,
                    mEstimatedCost,
                    mActualCost,
                    mTags,
                    mCompletionStatus,
                    *mLastModified};
    Task task{mName,
              mEstimatedCost,
              mActualCost,
              *mUuid,
              mTags,
              mCompletionStatus,
              *mLastModified};
    mUuid = std::optional<std::string>{};
    mLastModified = std::optional<DateTime>{};
    return task;
}

TaskBuilder& TaskBuilder::withName(std::string name)
{
    mName = std::move(name);
    return *this;
}

TaskBuilder& TaskBuilder::withEstimatedCost(int cost)
{
    mEstimatedCost = cost;
    return *this;
}

TaskBuilder& TaskBuilder::withActualCost(int cost)
{
    mActualCost = cost;
    return *this;
}

TaskBuilder& TaskBuilder::withUuid(std::string uuid)
{
    mUuid = std::make_optional(std::move(uuid));
    return *this;
}

TaskBuilder& TaskBuilder::withTag(std::string tag)
{
    mTags.emplace_back(Tag{std::move(tag)});
    return *this;
}

TaskBuilder& TaskBuilder::withTag(Tag tag)
{
    mTags.emplace_back(std::move(tag));
    return *this;
}

TaskBuilder& TaskBuilder::withExplicitTags(std::list<Tag> tags)
{
    mTags = std::move(tags);
    return *this;
}

TaskBuilder& TaskBuilder::withLastModificationStamp(DateTime timeStamp)
{
    mLastModified = std::make_optional(std::move(timeStamp));
    return *this;
}

TaskBuilder& TaskBuilder::withCompletionStatus(bool completed)
{
    mCompletionStatus = completed;
    return *this;
}

} // namespace sprint_timer
