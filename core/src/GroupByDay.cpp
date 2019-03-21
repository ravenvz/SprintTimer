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

#include "core/GroupByDay.h"

namespace sprint_timer {

std::vector<GoalProgress>
GroupByDay::computeProgress(const dw::TimeSpan& period,
                            const std::vector<int>& actualProgress,
                            utils::WeekdaySelection workdays,
                            int workdayGoal) const
{
    auto labour = workday_outline(period, workdays);
    std::vector<GoalProgress> progress(actualProgress.size());
    std::transform(actualProgress.cbegin(),
                   actualProgress.cend(),
                   labour.cbegin(),
                   progress.begin(),
                   [workdayGoal](auto actualVal, auto isWorkday) {
                       return GoalProgress{workdayGoal * isWorkday, actualVal};
                   });
    return progress;
}

} // namespace sprint_timer
