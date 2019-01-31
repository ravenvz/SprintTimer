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
#ifndef SPRINT_TIMER_APP_GOALPROGRESS_H
#define SPRINT_TIMER_APP_GOALPROGRESS_H

#include <optional>

namespace sprint_timer {

class GoalProgress {
public:
    GoalProgress();

    GoalProgress(int expected, int actual);

    int estimated() const;

    int actual() const;

    std::optional<double> percentage() const;

private:
    int expected_{0};
    int actual_{0};
};

bool operator==(const GoalProgress& lhs, const GoalProgress& rhs);

std::ostream& operator<<(std::ostream& os, const GoalProgress& progress);

} // namespace sprint_timer

#endif // SPRINT_TIMER_APP_GOALPROGRESS_H
