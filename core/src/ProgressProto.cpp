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

#include "core/ProgressProto.h"

namespace sprint_timer {

ProgressOverPeriod::ProgressOverPeriod(dw::DateTimeRange period,
                                       const Distribution<int>& actualProgress,
                                       utils::WeekdaySelection workdays,
                                       const GroupingStrategy& groupingStrategy,
                                       int workdayGoal)
    : period_{period}
    , workdays_{workdays}
    , actual_{actualProgress.getTotal()}
{
    const std::vector<int> dist = actualProgress.getDistributionVector();
    progress_
        = groupingStrategy.computeProgress(period, dist, workdays, workdayGoal);
    numWorkBins_ = std::accumulate(progress_.cbegin(),
                                   progress_.cend(),
                                   0,
                                   [](int acc, const GoalProgress& p) {
                                       return acc + (p.estimated() ? 1 : 0);
                                   });
    estimated_ = std::accumulate(
        progress_.cbegin(),
        progress_.cend(),
        0,
        [](int acc, const GoalProgress& p) { return acc + p.estimated(); });
}

std::optional<double> ProgressOverPeriod::percentage() const
{
    return numWorkBins_ == 0
        ? std::optional<double>{}
        : static_cast<double>(actual()) * 100 / estimated();
}

int ProgressOverPeriod::estimated() const { return estimated_; }

int ProgressOverPeriod::actual() const { return actual_; }

std::optional<double> ProgressOverPeriod::averagePerGroupPeriod() const
{
    return numWorkBins_ == 0 ? std::optional<double>{}
                             : static_cast<double>(actual()) / numWorkBins_;
}

GoalProgress ProgressOverPeriod::getValue(size_t binNumber) const
{
    return progress_[binNumber];
}

size_t ProgressOverPeriod::size() const { return progress_.size(); }

bool ProgressOverPeriod::isOverwork() const { return difference() < 0; }

int ProgressOverPeriod::difference() const { return estimated() - actual(); }

} // namespace sprint_timer
