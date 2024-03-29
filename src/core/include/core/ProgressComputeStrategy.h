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
#ifndef GROUPBYPERIODSTRATEGY_H_2JRBSIB5
#define GROUPBYPERIODSTRATEGY_H_2JRBSIB5

#include "core/GoalProgress.h"
#include "core/WorkSchedule.h"
#include "date_wrapper/date_wrapper.h"
#include <vector>

namespace sprint_timer {

class ProgressComputeStrategy {
public:
    virtual ~ProgressComputeStrategy() = default;

    virtual std::vector<GoalProgress>
    computeProgress(const dw::DateRange& dateRange,
                    const std::vector<int>& actualProgress,
                    const WorkSchedule& workSchedule) const = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: GROUPBYPERIODSTRATEGY_H_2JRBSIB5 */
