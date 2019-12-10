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
#include "core/GroupByMonth.h"

namespace sprint_timer {

std::vector<GoalProgress>
GroupByMonth::computeProgress(const dw::DateRange& dateRange,
                              const std::vector<int>& actualProgress,
                              const WorkdayTracker& workdayTracker) const
{
    using namespace dw;
    std::vector<int> labour;
    std::vector<GoalProgress> progress;
    progress.reserve(actualProgress.size());
    auto actualIt = cbegin(actualProgress);

    // To compute goal we break dateRange into months, taking in account that
    // it can start and end with random day of month.
    auto start = dateRange.start();
    const auto lastDay = dateRange.finish();
    auto next_finish = [&start, lastDay]() {
        return std::min(last_day_of_month(start), lastDay);
    };

    for (auto finish = next_finish(); start < lastDay;
         start = finish + Days{1}, finish = next_finish()) {
        const DateRange monthChunk{start, finish};
        if (actualIt != cend(actualProgress)) {
            progress.emplace_back(goalFor(workdayTracker, monthChunk),
                                  *actualIt);
            ++actualIt;
        }
        else {
            progress.emplace_back(goalFor(workdayTracker, monthChunk), 0);
        }
    }

    return progress;
}

} // namespace sprint_timer
