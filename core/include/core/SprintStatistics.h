/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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

class SprintStatItem {

public:
    SprintStatItem(const std::vector<Sprint>& sprints,
                   const dw::TimeSpan& timeInterval);

    SprintStatItem(const SprintStatItem&) = default;

    ~SprintStatItem() = default;

    Distribution<double> dailyDistribution() const;

    Distribution<double> weekdayDistribution() const;

    Distribution<double> worktimeDistribution() const;

    std::vector<Sprint> sprints() const;

private:
    const dw::TimeSpan timeSpan;
    const std::vector<Sprint> mSprints;

    std::vector<double> computeDailyDistribution() const;

    std::vector<double> computeWeekdayDistribution() const;

    std::vector<double> computeWorkTimeDistribution() const;

    std::vector<int> countWeekdays() const;
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

DayPart dayPart(const dw::TimeSpan& timeSpan);

std::string dayPartName(unsigned dayPart);

std::string dayPartName(DayPart dayPart);

std::string dayPartHours(DayPart dayPart);

std::string dayPartHours(unsigned dayPart);

} // namespace DatePart


#endif // SPRINTSTATISTICS_H
