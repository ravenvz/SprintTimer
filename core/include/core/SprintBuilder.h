/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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

#ifndef SPRINT_TIMER_SPRINTBUILDER_H
#define SPRINT_TIMER_SPRINTBUILDER_H

#include "core/entities/Sprint.h"
#include "core/utils/OptionalPl.h"
#include <stdexcept>

class SprintBuilderError : public std::runtime_error {
public:
    SprintBuilderError(const std::string& message)
        : std::runtime_error{message}
    {
    }
};

class SprintBuilder {

public:
    Sprint build();
    SprintBuilder& withName(const std::string& name);
    SprintBuilder& withUuid(const std::string& uuid);
    SprintBuilder& withTaskUuid(const std::string& uuid);
    SprintBuilder& withTag(const Tag& tag);
    SprintBuilder& withTag(const std::string& tag);
    SprintBuilder& withTimeSpan(const dw::TimeSpan& timeSpan);
    SprintBuilder& forTask(const Task& task);
    SprintBuilder& withExplicitTags(const std::list<Tag>& tags);

private:
    std::string mName;
    optional<std::string> mUuid;
    optional<std::string> mTaskUuid;
    std::list<Tag> mTags;
    optional<dw::TimeSpan> mTimeSpan;
};


#endif //SPRINT_TIMER_SPRINTBUILDER_H
