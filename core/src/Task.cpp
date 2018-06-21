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
#include "core/entities/Task.h"
#include <iostream>
#include <regex>

namespace sprint_timer::entities {

using dw::DateTime;

// static
BoostUUIDGenerator Task::generator;

Task::Task(std::string name,
           int estimatedCost,
           int actualCost,
           std::list<Tag> tags,
           bool completed,
           const DateTime& lastModified)
    : Task{std::move(name),
           estimatedCost,
           actualCost,
           generator.generateUUID(),
           std::move(tags),
           completed,
           lastModified}
{
}

Task::Task(std::string name,
           int estimatedCost,
           int actualCost,
           std::string uuid,
           std::list<Tag> tags,
           bool completed,
           const DateTime& lastModified)
    : mName{std::move(name)}
    , mEstimatedCost{estimatedCost}
    , mActualCost{actualCost}
    , mUuid{uuid}
    , mTags{std::move(tags)}
    , mCompleted{completed}
    , mLastModified{lastModified}
{
}

Task::Task(std::string encodedDescription)
    : mUuid{generator.generateUUID()}
    , mLastModified{DateTime::currentDateTimeLocal()}
{
    decodeDescription(std::move(encodedDescription));
}

std::string Task::estimatedPrefix = std::string{"*"};

std::string Task::name() const { return mName; }

bool Task::isCompleted() const { return mCompleted; }

int Task::estimatedCost() const { return mEstimatedCost; }

int Task::actualCost() const { return mActualCost; }

std::string Task::uuid() const { return mUuid; }

std::list<Tag> Task::tags() const { return mTags; }

DateTime Task::lastModified() const { return mLastModified; }

void Task::setName(const std::string& name) { mName = name; }

void Task::setCompleted(bool completed) { mCompleted = completed; }

void Task::setEstimatedCost(int numSprints) { mEstimatedCost = numSprints; }

void Task::setTags(const std::list<Tag>& newTags) { mTags = newTags; }

void Task::setActualCost(int numSprints) { mActualCost = numSprints; }

void Task::setModifiedTimeStamp(const DateTime& timeStamp)
{
    mLastModified = timeStamp;
}

void Task::decodeDescription(std::string&& encodedDescription)
{
    std::regex tagRegex{"^" + Tag::prefix + R"(\w+)"};
    std::regex estimatedRegex{"^\\" + estimatedPrefix + R"(\w+)"};
    std::regex anyNonWhitespace{"\\S+"};

    std::sregex_iterator words_begin{
        encodedDescription.begin(), encodedDescription.end(), anyNonWhitespace};
    std::sregex_iterator words_end;

    std::vector<std::string> nameParts;

    for (auto it = words_begin; it != words_end; ++it) {
        std::string word{it->str()};
        if (std::regex_match(word, tagRegex)) {
            mTags.push_back(Tag{word.substr(1)});
        }
        else if (std::regex_match(word, estimatedRegex)) {
            mEstimatedCost = stoi(word.substr(1));
        }
        else {
            nameParts.push_back(word);
        }
    }

    mName = utils::join(nameParts, " ");
}

std::ostream& operator<<(std::ostream& os, const Task& task)
{
    os << prefixTags(task.mTags);
    if (!task.mTags.empty())
        os << " ";
    os << task.mName << " ";
    os << task.mActualCost << "/" << task.mEstimatedCost << " ";
    os << "Uuid: " << task.mUuid;
    return os;
}

bool operator==(const Task& lhs, const Task& rhs)
{
    return lhs.uuid() == rhs.uuid() && lhs.name() == rhs.name()
        && lhs.estimatedCost() == rhs.estimatedCost()
        && lhs.actualCost() == rhs.actualCost() && lhs.tags() == rhs.tags()
        && lhs.isCompleted() == rhs.isCompleted()
        && lhs.lastModified() == rhs.lastModified();
}

} // namespace sprint_timer::entities
