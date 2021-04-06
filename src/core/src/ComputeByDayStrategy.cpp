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
#include "core/ComputeByDayStrategy.h"

namespace sprint_timer {

std::vector<GoalProgress>
ComputeByDayStrategy::computeProgress(const dw::DateRange& dateRange,
                                      const std::vector<int>& actualProgress,
                                      const WorkSchedule& workSchedule) const
{
    using namespace dw;
    std::vector<GoalProgress> progress;
    progress.reserve(static_cast<size_t>(dateRange.duration().count()));
    auto actualIt = cbegin(actualProgress);

    for (auto day = dateRange.start(); day <= dateRange.finish();
         day = day + Days{1}) {
        if (actualIt != cend(actualProgress)) {
            progress.emplace_back(
                GoalProgress::Estimated{workSchedule.goal(day)},
                GoalProgress::Actual{*actualIt});
            ++actualIt;
        }
        else
            progress.emplace_back(
                GoalProgress::Estimated{workSchedule.goal(day)},
                GoalProgress::Actual{0});
    }

    return progress;
}

} // namespace sprint_timer
