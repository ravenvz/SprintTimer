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
#include "core/ComputeByWeekStrategy.h"
#include "core/IConfig.h"

namespace {

dw::Weekday weekday_before(dw::Weekday);

} // namespace

namespace sprint_timer {

ComputeByWeekStrategy::ComputeByWeekStrategy(dw::Weekday firstDayOfWeek_)
    : firstDayOfWeek{firstDayOfWeek_}
{
}

std::vector<GoalProgress>
ComputeByWeekStrategy::computeProgress(const dw::DateRange& dateRange,
                             const std::vector<int>& actualProgress,
                             const WorkSchedule& workSchedule) const
{
    using namespace dw;
    std::vector<GoalProgress> progress;
    progress.reserve(actualProgress.size());
    auto actualIt = cbegin(actualProgress);

    const dw::Weekday lastDayOfWeek{weekday_before(firstDayOfWeek)};

    // To compute goal we are breaking period into intervals of weeks, taking in
    // account that period can start and end with random weekday
    auto start = dateRange.start();
    const auto lastDay = dateRange.finish();
    auto next_finish = [&start, lastDay, lastDayOfWeek]() {
        return std::min(dw::next_weekday(start, lastDayOfWeek), lastDay);
    };

    for (auto finish = next_finish(); start <= lastDay;
         start = finish + Days{1}, finish = next_finish()) {
        const DateRange weekChunk{start, finish};
        if (actualIt != cend(actualProgress)) {
            progress.emplace_back(
                GoalProgress::Estimated{goalFor(workSchedule, weekChunk)},
                GoalProgress::Actual{*actualIt});
            ++actualIt;
        }
        else
            progress.emplace_back(
                GoalProgress::Estimated{goalFor(workSchedule, weekChunk)},
                GoalProgress::Actual{0});
    }

    return progress;
}

} // namespace sprint_timer

namespace {

dw::Weekday weekday_before(dw::Weekday weekday)
{
    if (weekday == dw::Weekday::Monday)
        return dw::Weekday::Sunday;
    return static_cast<dw::Weekday>(static_cast<int>(weekday) - 1);
}

} // namespace
