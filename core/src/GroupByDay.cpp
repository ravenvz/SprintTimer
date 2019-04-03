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
GroupByDay::computeProgress(const dw::DateRange& period,
                            const std::vector<int>& actualProgress,
                            const WorkdayTracker& workdayTracker,
                            int workdayGoal) const
{
    using namespace dw;
    std::vector<GoalProgress> progress;
    progress.reserve(actualProgress.size());
    auto actualIt = cbegin(actualProgress);

    for (auto day = period.start(); day <= period.finish();
         day = day + Days{1}, ++actualIt) {
        const bool isWorkday{workdayTracker.isWorkday(day)};
        progress.emplace_back(workdayGoal * isWorkday, *actualIt);
    }

    return progress;
}

} // namespace sprint_timer
