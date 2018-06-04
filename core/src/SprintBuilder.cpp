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

#include "core/SprintBuilder.h"

Sprint SprintBuilder::build()
{
    using namespace std::chrono;
    if (!mTaskUuid)
        throw SprintBuilderError{"Can't build Sprint without Task uuid"};
    // TODO remove magic constant. It should be taken from config anyway
    if (!mTimeSpan)
        throw SprintBuilderError{"Can't build Sprint without timespan"};
    if (!mUuid)
        return Sprint{mName, *mTimeSpan, mTags, *mTaskUuid};
    Sprint sprint{mName, *mTimeSpan, mTags, *mUuid, *mTaskUuid};
    mUuid = std::nullopt;
    return sprint;
}

SprintBuilder& SprintBuilder::withName(const std::string& name)
{
    mName = name;
    return *this;
}

SprintBuilder& SprintBuilder::withUuid(const std::string& uuid)
{
    mUuid = std::make_optional(uuid);
    return *this;
}

SprintBuilder& SprintBuilder::withTaskUuid(const std::string& uuid)
{
    mTaskUuid = std::make_optional(uuid);
    return *this;
}

SprintBuilder& SprintBuilder::withTag(const Tag& tag)
{
    mTags.emplace_back(tag);
    return *this;
}

SprintBuilder& SprintBuilder::withTag(const std::string& tag)
{
    mTags.emplace_back(Tag{tag});
    return *this;
}

SprintBuilder& SprintBuilder::withTimeSpan(const dw::TimeSpan& timeSpan)
{
    mTimeSpan = std::make_optional(timeSpan);
    return *this;
}

SprintBuilder& SprintBuilder::forTask(const Task& task)
{
    mName = task.name();
    mTaskUuid = std::make_optional(task.uuid());
    mTags = task.tags();
    return *this;
}

SprintBuilder& SprintBuilder::withExplicitTags(const std::list<Tag>& tags)
{
    mTags = tags;
    return *this;
}

