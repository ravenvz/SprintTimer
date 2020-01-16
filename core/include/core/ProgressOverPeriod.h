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
#ifndef PROGRESS_H_LE0ZZW37
#define PROGRESS_H_LE0ZZW37

#include "core/GoalProgress.h"
#include "core/GroupByPeriodStrategy.h"
#include "core/WorkSchedule.h"
#include <date_wrapper/date_wrapper.h>
#include <iostream>
#include <numeric>
#include <optional>

namespace sprint_timer {

class ProgressOverPeriod {
public:
    ProgressOverPeriod(const dw::DateRange& period,
                       const std::vector<int>& actualProgress,
                       const WorkSchedule& workSchedule,
                       const GroupByPeriodStrategy& groupByPeriodStrategy);

    std::optional<double> percentage() const;

    int estimated() const;

    int actual() const;

    std::optional<double> averagePerGroupPeriod() const;

    GoalProgress getValue(size_t binNumber) const;

    size_t size() const;

    bool isOverwork() const;

    int difference() const;

private:
    const int actual_;
    std::vector<GoalProgress> progress_;
    int numWorkBins_;
    int estimated_;
};

} // namespace sprint_timer

#endif /* end of include guard: PROGRESS_H_LE0ZZW37 */

