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
#ifndef GROUPBYWEEK_H_UY9VBON4
#define GROUPBYWEEK_H_UY9VBON4

#include "core/IConfig.h"
#include "core/ProgressGroupingStrategy.h"

namespace sprint_timer {

class GroupByWeek : public ProgressGroupingStrategy {
public:
    explicit GroupByWeek(const IConfig& applicationSettings);

    std::vector<GoalProgress>
    computeProgress(const dw::DateRange& dateRange,
                    const std::vector<int>& actualProgress,
                    const WorkSchedule& workSchedule) const override;

private:
    const IConfig& applicationSettings;
};

} // namespace sprint_timer

#endif /* end of include guard: GROUPBYWEEK_H_UY9VBON4 */
