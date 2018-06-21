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

#ifndef SPRINT_TIMER_SPRINTBUILDER_H
#define SPRINT_TIMER_SPRINTBUILDER_H

#include "core/entities/Task.h"
#include <optional>
#include <stdexcept>

namespace sprint_timer {

class SprintBuilderError : public std::runtime_error {
public:
    SprintBuilderError(const std::string& message)
        : std::runtime_error{message}
    {
    }
};

class SprintBuilder {

public:
    entities::Sprint build();
    SprintBuilder& withName(const std::string& name);
    SprintBuilder& withUuid(const std::string& uuid);
    SprintBuilder& withTaskUuid(const std::string& uuid);
    SprintBuilder& withTag(const entities::Tag& tag);
    SprintBuilder& withTag(const std::string& tag);
    SprintBuilder& withTimeSpan(const dw::TimeSpan& timeSpan);
    SprintBuilder& forTask(const entities::Task& task);
    SprintBuilder& withExplicitTags(const std::list<entities::Tag>& tags);

private:
    std::string mName;
    std::optional<std::string> mUuid;
    std::optional<std::string> mTaskUuid;
    std::list<entities::Tag> mTags;
    std::optional<dw::TimeSpan> mTimeSpan;
};

} // namespace sprint_timer

#endif // SPRINT_TIMER_SPRINTBUILDER_H
