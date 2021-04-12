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
#ifndef PROGRESS_H_LE0ZZW37
#define PROGRESS_H_LE0ZZW37

#include "core/GoalProgress.h"
#include "core/ProgressComputeStrategy.h"
#include "core/WorkSchedule.h"
#include "date_wrapper/date_wrapper.h"
#include <iostream>
#include <numeric>
#include <optional>

namespace sprint_timer {

class ProgressOverPeriod {
public:
    ProgressOverPeriod(std::vector<GoalProgress>&& data);

    std::optional<double> percentage() const;

    int estimated() const;

    int actual() const;

    std::optional<double> averagePerGroupPeriod() const;

    GoalProgress getValue(size_t binNumber) const;

    size_t size() const;

    bool isOverwork() const;

    int difference() const;

private:
    std::vector<GoalProgress> data;
    int totalActual;
    int totalEstimated;
    int numBins;
};

} // namespace sprint_timer

#endif /* end of include guard: PROGRESS_H_LE0ZZW37 */

