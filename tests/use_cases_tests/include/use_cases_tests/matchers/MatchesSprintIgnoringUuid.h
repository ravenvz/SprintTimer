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
#ifndef MATCHESSPRINTIGNORINGUUID_H_8ERPY9CG
#define MATCHESSPRINTIGNORINGUUID_H_8ERPY9CG

#include "core/use_cases/SprintDTO.h"

namespace matchers {

struct MatchesSprintIgnoringUuid {
    using match_t = sprint_timer::use_cases::SprintDTO;

    explicit MatchesSprintIgnoringUuid(const match_t&);

    bool operator()(const match_t&) const;

private:
    const match_t& expected;
};

} // namespace matchers

#endif /* end of include guard: MATCHESSPRINTIGNORINGUUID_H_8ERPY9CG */
