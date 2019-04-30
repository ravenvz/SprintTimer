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
#include "core/GroupByWeek.h"
#include <core/IConfig.h>
#include <core/utils/WeekdaySelection.h>

namespace {

dw::DateRange nWeeksBackTillNow(int numWeeks, dw::Weekday firstDayOfWeek);

} // namespace

namespace sprint_timer {

GroupByWeek::GroupByWeek(int numWeeks_, const IConfig& applicationSettings_)
    : applicationSettings{applicationSettings_}
    , numWeeks{numWeeks_}
{
}

std::vector<GoalProgress>
GroupByWeek::computeProgress(const std::vector<int>& actualProgress,
                             const WorkdayTracker& workdayTracker) const
{
    std::vector<GoalProgress> progress;
    progress.reserve(actualProgress.size());
    auto actualIt = actualProgress.cbegin();

    int goalForCurrentWeek{0};

    const auto period = dateRange();
    const dw::Weekday firstDay{applicationSettings.firstDayOfWeek()};

    for (auto day = period.start(); day <= period.finish();
         day = day + dw::Days{1}) {
        if (day == period.finish()) {
            goalForCurrentWeek += workdayTracker.goal(day);
            progress.emplace_back(goalForCurrentWeek, *actualIt);
            break;
        }
        if ((weekday(day) == firstDay && day != period.start())) {
            progress.emplace_back(goalForCurrentWeek, *actualIt);
            ++actualIt;
            goalForCurrentWeek = 0;
        }
        goalForCurrentWeek += workdayTracker.goal(day);
    }

    return progress;
}

dw::DateRange GroupByWeek::dateRange() const
{
    return nWeeksBackTillNow(numWeeks, applicationSettings.firstDayOfWeek());
}

} // namespace sprint_timer

namespace {

dw::DateRange nWeeksBackTillNow(int numWeeks, dw::Weekday firstDayOfWeek)
{
    using namespace dw;
    auto now = current_date_local();
    const auto from = prev_weekday(now - Weeks{numWeeks - 1},
                                   static_cast<dw::Weekday>(firstDayOfWeek));
    const auto lastDayOfWeek{firstDayOfWeek == dw::Weekday::Monday
                                 ? dw::Weekday::Sunday
                                 : dw::Weekday::Saturday};
    const auto to = next_weekday(now, lastDayOfWeek);
    return {from, to};
}

} // namespace
