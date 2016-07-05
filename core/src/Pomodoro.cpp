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
#include "core/entities/Pomodoro.h"
#include "core/StringUtils.h"
#include <algorithm>


// static
BoostUUIDGenerator Pomodoro::generator;

// TODO too much constructors that instantiate partial pomodoros.

Pomodoro::Pomodoro(const TimeSpan& timeSpan)
    : mInterval{timeSpan}
    , mUuid{generator.generateUUID()}
{
}

Pomodoro::Pomodoro(const std::string& taskName,
                   const TimeSpan& timeSpan,
                   const std::list<Tag>& tags)
    : mName{taskName}
    , mInterval{timeSpan}
    , mUuid{generator.generateUUID()}
    , mTags{tags}
{
}

Pomodoro::Pomodoro(const std::string& taskName,
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

Pomodoro::Pomodoro(const Task& task, const TimeSpan& timeSpan)
    : mName{task.name()}
    , mInterval{timeSpan}
    , mUuid{generator.generateUUID()}
    , mTaskUuid{task.uuid()}
    , mTags{task.tags()}
{
}

Pomodoro::Pomodoro(const std::string& taskUuid, const TimeSpan& timeSpan)
    : mInterval{timeSpan}
    , mUuid{generator.generateUUID()}
    , mTaskUuid{taskUuid}
{
}

std::string Pomodoro::name() const { return mName; }

DateTime Pomodoro::startTime() const { return mInterval.startTime; }

DateTime Pomodoro::finishTime() const { return mInterval.finishTime; }

TimeSpan Pomodoro::timeSpan() const { return mInterval; }

std::string Pomodoro::uuid() const { return mUuid; }

std::string Pomodoro::taskUuid() const { return mTaskUuid; }

std::list<Tag> Pomodoro::tags() const { return mTags; }

std::string Pomodoro::toString() const
{
    std::vector<std::string> prefixedTags;
    std::vector<std::string> parts;

    std::transform(mTags.cbegin(),
                   mTags.cend(),
                   std::back_inserter(prefixedTags),
                   [](const auto& elem) { return elem.nameWithPrefix(); });
    // std::copy(mTags.cbegin(), mTags.cend(),
    // std::back_inserter(prefixedTags));
    //
    // std::for_each(prefixedTags.begin(), prefixedTags.end(), [&](auto& elem) {
    //     elem.insert(0, Task::tagPrefix);
    // });

    parts.push_back(mInterval.toTimeString());
    parts.push_back(StringUtils::join(prefixedTags, " "));
    parts.push_back(mName);

    return StringUtils::join(parts, " ");
}
