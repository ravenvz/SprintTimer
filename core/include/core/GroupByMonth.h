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
#ifndef SPRINT_TIMER_APP_GROUPBYMONTH_H
#define SPRINT_TIMER_APP_GROUPBYMONTH_H

#include "ProgressProto.h"

namespace sprint_timer {

class GroupByMonth : public GroupingStrategy {
public:
    std::vector<GoalProgress>
    computeProgress(const dw::TimeSpan& period,
                    const std::vector<int>& actualProgress,
                    utils::WeekdaySelection workdays,
                    int workdayGoal) const override
    {
        std::vector<int> labour;
        std::vector<GoalProgress> progress;
        progress.reserve(actualProgress.size());
        auto actualIt = cbegin(actualProgress);

        int numWorkdays{0};
        unsigned currentMonth{period.start().month()};

        const dw::DateTime stop = period.finish().add(dw::DateTime::Days{1});
        for (auto day = period.start(); day < stop;
             day = day.add(dw::DateTime::Days{1})) {
            if (day.month() != currentMonth) {
                currentMonth = day.month();
                progress.emplace_back(workdayGoal * numWorkdays, *actualIt);
                ++actualIt;
                numWorkdays = 0;
            }
            if (workdays.isSelected(day.dayOfWeek()))
                ++numWorkdays;
        }
        progress.emplace_back(workdayGoal * numWorkdays, *actualIt);

        return progress;
    }
};

} // namespace sprint_timer

#endif // SPRINT_TIMER_APP_GROUPBYMONTH_H