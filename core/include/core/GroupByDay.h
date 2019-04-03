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
#ifndef GROUPBYDAY_H_6EWBVYZU
#define GROUPBYDAY_H_6EWBVYZU

#include "core/GroupingStrategy.h"

namespace sprint_timer {

class GroupByDay : public GroupingStrategy {
public:
    std::vector<GoalProgress>
    computeProgress(const dw::DateRange& period,
                    const std::vector<int>& actualProgress,
                    const WorkdayTracker& workdayTracker,
                    int workdayGoal) const override;
};

} // namespace sprint_timer

#endif /* end of include guard: GROUPBYDAY_H_6EWBVYZU */
