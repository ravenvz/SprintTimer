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
#ifndef SPRINTSTATISTICS_H
#define SPRINTSTATISTICS_H

#include "Distribution.h"
#include "core/entities/Sprint.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

// TODO refactor this ugly mess

using TagCount = std::pair<Tag, double>;


/* Datastructure that stores number of completed sprints for each tag. */
class TagDistribution {
public:
    using TagSprintMap = std::unordered_map<Tag, std::vector<Sprint>>;

    TagDistribution(const std::vector<Sprint>& sprints, int numTopTags);

    TagDistribution() = default;

    /* Return vector of (tag, number of finished sprints) pairs.
     * Only top tags are considered (specified by numTopTags). Finished
     * sprints for all other tags are summed up, stored in empty '' tag and
     * appended to the returned vector.
     * Returned vector is sorted in descending order of number of completed sprints,
     * except for last item that contains number of completed sprints
     * for all other tags (if it is present). */
    std::vector<TagCount> topTagsDistribution() const;

    /* Return vector of sprints that have given tag. */
    std::vector<Sprint> sprintsWithTag(const Tag &tag) const;

    /* Return prints for n-th top tag. */
    std::vector<Sprint> sprintsForNthTopTag(size_t n) const;

    /* Return n-th top tag name. */
    std::string getNthTopTagName(size_t n) const;

private:
    TagSprintMap tagSprintMap;
    std::unordered_map<Tag, std::vector<Sprint>> sprintsForTag;
    std::vector<TagCount> sliceData;
    std::unordered_map<size_t, Tag> sliceIndexMap;
    size_t numTopTags;

    void buildDistribution();

    void reduceTailToSum();

    void buildIndexMap();
};

class SprintStatItem {

public:
    SprintStatItem(const std::vector<Sprint>& sprints,
                     const TimeSpan& timeInterval);

    SprintStatItem(const SprintStatItem&) = default;

    ~SprintStatItem() = default;

    Distribution<double> dailyDistribution() const;

    Distribution<double> weekdayDistribution() const;

    Distribution<double> worktimeDistribution() const;

    std::vector<Sprint> sprints() const;

private:
    const TimeSpan timeSpan;
    const std::vector<Sprint> mSprints;

    std::vector<double> computeDailyDistribution() const;

    std::vector<double> computeWeekdayDistribution() const;

    std::vector<double> computeWorkTimeDistribution() const;

    std::vector<int> countWeekdays() const;
};

#endif // SPRINTSTATISTICS_H
