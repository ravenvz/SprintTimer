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

namespace {

dw::DateRange nMonthsBackTillNow(int numMonths);

} // namespace

namespace sprint_timer {

GroupByMonth::GroupByMonth(int numMonths)
    : period{nMonthsBackTillNow(numMonths)}
{
}

std::vector<GoalProgress>
GroupByMonth::computeProgress(const std::vector<int>& actualProgress,
                              const WorkdayTracker& workdayTracker) const
{
    using namespace dw;
    std::vector<int> labour;
    std::vector<GoalProgress> progress;
    progress.reserve(actualProgress.size());
    auto actualIt = cbegin(actualProgress);

    int goalForCurrentMonth{0};
    auto currentMonth = period.start().month();

    const Date stop = period.finish() + Days{1};
    for (auto day = period.start(); day < stop; day = day + Days{1}) {
        if (day.month() != currentMonth) {
            currentMonth = day.month();
            progress.emplace_back(goalForCurrentMonth, *actualIt);
            ++actualIt;
            goalForCurrentMonth = 0;
        }
        goalForCurrentMonth += workdayTracker.goal(day);
    }
    progress.emplace_back(goalForCurrentMonth, *actualIt);

    return progress;
}

dw::DateRange GroupByMonth::dateRange() const { return period; }

} // namespace sprint_timer

namespace {

dw::DateRange nMonthsBackTillNow(int numMonths)
{
    using namespace dw;
    const auto now = current_date_local();
    const auto months_back = now - Months{numMonths - 1};
    const auto to = last_day_of_month(now);
    const auto from = Date{months_back.year(), months_back.month(), Day{1}};
    return DateRange{from, to};
}

} // namespace
