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
#include "core/GoalProgress.h"
#include "core/ProgressOverPeriod.h"

namespace sprint_timer {

GoalProgress::GoalProgress()
    : expected_{0}
    , actual_{0}
{
}

GoalProgress::GoalProgress(Estimated expected, Actual actual)
    : expected_{expected.value}
    , actual_{actual.value}
{
}

int GoalProgress::actual() const { return actual_; }

int GoalProgress::estimated() const { return expected_; }

std::optional<double> GoalProgress::percentage() const
{
    return expected_ == 0 ? std::optional<double>{}
                          : static_cast<double>(actual_) * 100 / expected_;
}

bool operator==(const GoalProgress& lhs, const GoalProgress& rhs)
{
    return lhs.estimated() == rhs.estimated() && lhs.actual() == rhs.actual();
}

std::ostream& operator<<(std::ostream& os, const GoalProgress& progress)
{
    os << "{" << progress.estimated() << ", " << progress.actual() << "}"
       << '\n';
    return os;
}

} // namespace sprint_timer

