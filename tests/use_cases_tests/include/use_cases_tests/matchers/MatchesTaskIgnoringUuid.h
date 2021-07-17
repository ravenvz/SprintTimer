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
#ifndef MATCHESTASKIGNORINGUUID_H_FACBNU37
#define MATCHESTASKIGNORINGUUID_H_FACBNU37

#include "core/use_cases/TaskDTO.h"

namespace matchers {

struct MatchesTaskIgnoringUuid {
    explicit MatchesTaskIgnoringUuid(
        const sprint_timer::use_cases::TaskDTO& taskDTO);

    bool operator()(const sprint_timer::use_cases::TaskDTO& task) const;

private:
    const sprint_timer::use_cases::TaskDTO& expected;
};

} // namespace matchers

#endif /* end of include guard: MATCHESTASKIGNORINGUUID_H_FACBNU37 */
