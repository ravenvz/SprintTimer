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
#ifndef SPRINTSTATISTICS_H
#define SPRINTSTATISTICS_H

#include "Distribution.h"
#include "core/entities/Sprint.h"
#include <algorithm>
#include <vector>

namespace sprint_timer {

/* This auxilary datastructure holds distributions of statistical data
 * for Sprints within given time range. */
class SprintStatItem {

public:
    /* Sprints that have start time dates that are not within timeSpan
     * dates will be ignored.
     */
    SprintStatItem(const std::vector<entities::Sprint>& sprints,
                   const dw::DateTimeRange& timeSpan);

    const Distribution<double>& dailyDistribution() const;

    const Distribution<double>& weekdayDistribution() const;

    const Distribution<double>& worktimeDistribution() const;

private:
    Distribution<double> dailyDistribution_;
    Distribution<double> weekdayDistribution_;
    Distribution<double> worktimeDistribution_;
};


namespace DayPart {

    constexpr size_t numParts{6};

    /* Represent a day partition
     *
     * Day has 6 4-hour parts:
     *      Midnight  22:00 - 2:00
     *      Night      2:00 - 6:00
     *      Morning    6:00 - 10:00
     *      Noon      10:00 - 14:00
     *      Afternoon 14:00 - 18:00
     *      Evening   18:00 - 22:00
     */
    enum class DayPart { Midnight, Night, Morning, Noon, Afternoon, Evening };

    std::string dayPartHours(unsigned dayPart);

    DayPart dayPart(const dw::DateTimeRange& timeSpan);

    std::string dayPartName(unsigned dayPart);

    std::string dayPartName(DayPart dayPart);

    std::string dayPartHours(DayPart dayPart);

    std::string dayPartHours(unsigned dayPart);

} // namespace DayPart


} // namespace sprint_timer

#endif // SPRINTSTATISTICS_H
