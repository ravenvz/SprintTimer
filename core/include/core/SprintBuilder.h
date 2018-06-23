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
    SprintBuilder& withName(std::string name);
    SprintBuilder& withUuid(std::string uuid);
    SprintBuilder& withTaskUuid(std::string uuid);
    SprintBuilder& withTag(entities::Tag tag);
    SprintBuilder& withTag(std::string tag);
    SprintBuilder& withTimeSpan(dw::TimeSpan timeSpan);
    SprintBuilder& forTask(const entities::Task& task);
    SprintBuilder& withExplicitTags(std::list<entities::Tag> tags);

private:
    std::string name_;
    std::optional<std::string> uuid_;
    std::optional<std::string> taskUuid_;
    std::list<entities::Tag> tags_;
    std::optional<dw::TimeSpan> timeSpan_;
};

} // namespace sprint_timer

#endif // SPRINT_TIMER_SPRINTBUILDER_H
