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
#include "core/GroupByWeek.h"

namespace sprint_timer {

GroupByWeek::GroupByWeek(dw::DateTime::Weekday firstDayOfWeek)
    : firstDay{firstDayOfWeek}
{
}

std::vector<GoalProgress>
GroupByWeek::computeProgress(const dw::TimeSpan& period,
                             const std::vector<int>& actualProgress,
                             utils::WeekdaySelection workdays,
                             int workdayGoal) const
{
    std::vector<GoalProgress> progress;
    progress.reserve(actualProgress.size());
    auto actualIt = actualProgress.cbegin();

    int numWorkdays{0};

    for (auto day = period.start(); day <= period.finish();
         day = day.add(dw::DateTime::Days{1})) {
        if (day == period.finish()) {
            if (workdays.isSelected(day.dayOfWeek()))
                ++numWorkdays;
            progress.emplace_back(numWorkdays * workdayGoal, *actualIt);
            break;
        }
        if ((day.dayOfWeek() == firstDay && day != period.start())) {
            progress.emplace_back(numWorkdays * workdayGoal, *actualIt);
            ++actualIt;
            numWorkdays = 0;
        }
        if (workdays.isSelected(day.dayOfWeek()))
            ++numWorkdays;
    }

    return progress;
}
} // namespace sprint_timer
