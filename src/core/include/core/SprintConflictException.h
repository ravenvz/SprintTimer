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
#ifndef SPRINTCONFLICTEXCEPTION_H
#define SPRINTCONFLICTEXCEPTION_H

#include "core/SprintTimerException.h"
#include "core/entities/Sprint.h"
#include <string>
#include <vector>

namespace sprint_timer {

class SprintConflictException : public SprintTimerException {
public:
    using conflicting_sprints_pair = std::pair<sprint_timer::entities::Sprint,
                                               sprint_timer::entities::Sprint>;

    explicit SprintConflictException(
        std::vector<conflicting_sprints_pair>&& conflictingSprints);

    const std::vector<conflicting_sprints_pair>& conflictingSprints() const;

private:
    std::vector<conflicting_sprints_pair> sprintPairs;
};

} // namespace sprint_timer

#endif /* end of include guard: SPRINTCONFLICTEXCEPTION_H */
