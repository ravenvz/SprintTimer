/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef POMODORO_POMODOROSTATISTICS_H
#define POMODORO_POMODOROSTATISTICS_H

#include "Distribution.h"
#include "core/entities/Sprint.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

// TODO refactor this ugly mess

using TagCount = std::pair<Tag, double>;


/* Datastructure that stores number of completed pomodoros for each tag. */
class TagDistribution {
public:
    using TagPomoMap = std::unordered_map<Tag, std::vector<Sprint>>;

    TagDistribution(const std::vector<Sprint>& pomodoros, int numTopTags);

    TagDistribution() = default;

    /* Return vector of (tag, number of completed pomodoros) pairs.
     * Only top tags are considered (specified by numTopTags). Completed
     * pomodoros for all other tags are summed up, stored in empty '' tag and
     * appended to the returned vector.
     * Returned vector is sorted in descending order of completed pomodoros,
     * except for last item that containes number of completed pomodoros
     * for all other tags (if it is present). */
    std::vector<TagCount> topTagsDistribution() const;

    /* Return vector of pomodoros that have given tag. */
    std::vector<Sprint> pomodorosWithTag(const Tag& tag) const;

    /* Return vector of pomodoros for n-th top tag. */
    std::vector<Sprint> pomodorosForNthTopTag(size_t n) const;

    /* Return n-th top tag name. */
    std::string getNthTopTagName(size_t n) const;

private:
    TagPomoMap map;
    std::unordered_map<Tag, std::vector<Sprint>> tagToPomodoroVec;
    std::vector<TagCount> sliceData;
    std::unordered_map<size_t, Tag> sliceIndexMap;
    size_t numTopTags;

    void buildDistribution();

    void reduceTailToSum();

    void buildIndexMap();
};

class PomodoroStatItem {

public:
    PomodoroStatItem(const std::vector<Sprint>& pomodoros,
                     const TimeSpan& timeInterval);

    PomodoroStatItem(const PomodoroStatItem&) = default;

    ~PomodoroStatItem() = default;

    Distribution<double> dailyDistribution() const;

    Distribution<double> weekdayDistribution() const;

    Distribution<double> worktimeDistribution() const;

    std::vector<Sprint> pomodoros() const;

private:
    const TimeSpan timeSpan;
    const std::vector<Sprint> pomos;

    std::vector<double> computeDailyDistribution() const;

    std::vector<double> computeWeekdayDistribution() const;

    std::vector<double> computeWorkTimeDistribution() const;

    std::vector<int> countWeekdays() const;
};

#endif // POMODORO_POMODOROSTATISTICS_H
