/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "core/use_cases/request_sprint_distribution/Common.h"

namespace {

struct TagsEqual {
    TagsEqual(const sprint_timer::entities::Tag& referenceTag)
        : referenceTag{referenceTag}
    {
    }

    bool operator()(const sprint_timer::entities::Tag& tag)
    {
        return tag == referenceTag;
    }

private:
    const sprint_timer::entities::Tag& referenceTag;
};

struct ContainsTag {
    ContainsTag(const sprint_timer::entities::Tag& referenceTag)
        : referenceTag{referenceTag}
    {
    }

    bool operator()(const sprint_timer::entities::Sprint& sprint)
    {
        const auto& tags = sprint.tags();
        return std::any_of(cbegin(tags), cend(tags), TagsEqual{referenceTag});
    }

private:
    const sprint_timer::entities::Tag& referenceTag;
};

} // namespace

namespace sprint_timer::use_cases {

std::vector<sprint_timer::entities::Sprint>
filterByTag(const std::vector<sprint_timer::entities::Sprint>& sprints,
            const sprint_timer::entities::Tag& tag)
{
    std::vector<sprint_timer::entities::Sprint> filteredSprints;
    std::copy_if(cbegin(sprints),
                 cend(sprints),
                 std::back_inserter(filteredSprints),
                 ContainsTag{tag});

    return filteredSprints;
}

} // namespace sprint_timer::use_cases

