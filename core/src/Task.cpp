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
#include "core/entities/Task.h"
#include <iostream>
#include <regex>


// static
BoostUUIDGenerator Task::generator;

Task::Task(std::string name,
                   int estimatedCost,
                   int actualCost,
                   std::list<Tag> tags,
                   bool completed)
    : mName(name)
    , mEstimatedCost(estimatedCost)
    , mActualCost(actualCost)
    , mUuid{generator.generateUUID()}
    , mTags(tags)
    , mCompleted(completed)
    , mLastModified{DateTime::currentDateTimeLocal()}
{
}

Task::Task(std::string name,
                   int estimatedCost,
                   int actualCost,
                   const std::string& uuid,
                   std::list<Tag> tags,
                   bool completed,
                   const DateTime& lastModified)
    : mName(name)
    , mEstimatedCost(estimatedCost)
    , mActualCost(actualCost)
    , mUuid{uuid}
    , mTags(tags)
    , mCompleted(completed)
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

void Task::setEstimatedCost(int numSprints)
{
    mEstimatedCost = numSprints;
}

void Task::setTags(const std::list<Tag>& newTags) { mTags = newTags; }

void Task::setActualCost(int numSprints)
{
    mActualCost = numSprints;
}

void Task::setModifiedTimeStamp(const DateTime& timeStamp)
{
    mLastModified = timeStamp;
}

std::string Task::tagsAsString() const
{
    std::list<std::string> prefixedTags;
    std::transform(mTags.cbegin(),
                   mTags.cend(),
                   std::back_inserter(prefixedTags),
                   [](const auto& elem) { return elem.nameWithPrefix(); });
    return StringUtils::join(prefixedTags.begin(), prefixedTags.end(), " ");
}


std::string Task::toString() const
{
    std::vector<std::string> parts;
    std::string result;
    parts.push_back(tagsAsString());
    parts.push_back(mName);
    parts.push_back(StringUtils::join(
        {std::to_string(mActualCost), std::to_string(mEstimatedCost)},
        "/"));
    return StringUtils::join(parts, " ");
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
            mTags.push_back(word.substr(1));
        }
        else if (std::regex_match(word, estimatedRegex)) {
            mEstimatedCost = stoi(word.substr(1));
        }
        else {
            nameParts.push_back(word);
        }
    }

    mName = StringUtils::join(nameParts, " ");
}
