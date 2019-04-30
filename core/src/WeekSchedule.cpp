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
#include "core/WeekSchedule.h"

namespace sprint_timer {

int WeekSchedule::targetGoal(const dw::Weekday& weekday) const
{
    return goals_[static_cast<size_t>(weekday)];
}

void WeekSchedule::setTargetGoal(const dw::Weekday& weekday, int goal)
{
    goals_[static_cast<size_t>(weekday)] = goal;
}

bool WeekSchedule::isWorkday(const dw::Weekday& weekday) const
{
    return goals_[static_cast<size_t>(weekday)] > 0;
}

bool operator==(const WeekSchedule& lhs, const WeekSchedule& rhs)
{
    for (size_t i = 0; i < 7; ++i) {
        if (lhs.targetGoal(static_cast<dw::Weekday>(i))
            != rhs.targetGoal(static_cast<dw::Weekday>(i)))
            return false;
    }
    return true;
}

} // namespace sprint_timer

