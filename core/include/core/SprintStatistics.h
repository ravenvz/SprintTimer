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


/* Responsible for arranging sprints by their tags in order of tag frequency.
 *
 * Tag that has most sprints is placed at the top position; top position is a
 * position with index 0.
 * Tags with lower frequencies will take positions with
 * increasing index.
 * Tags with lowest frequencies are merged together and are placed
 * at the very bottom.
 * Maximum size of the top is specified at class construction.
 * Note, that actual size might be lower than maximum, because not all top
 * position might be filled.
 *
 * In case when two or more tags have same frequency, the tie is broken
 * arbitrarily.
 *
 * As an example, if we have some sprints with following tag frequencies:
 * (Tag1, 0.1), (Tag2 0.3), (Tag3, 0.2), (Tag4, 0.3), (Tag5, 0.1) and top
 * size is 4, the resulting top could look like:
 *
 * position     Tag name    Frequency
 *    0           Tag2         0.3
 *    1           Tag4         0.3
 *    2           Tag3         0.2
 *    3           others       0.1 + 0.1 = 0.2
 *
 *
 * */
class TagTop {
public:
    using TagFrequency = std::pair<Tag, double>;

    using TagSprints = std::unordered_map<Tag, std::vector<Sprint>>;

    TagTop(const std::vector<Sprint>& sprints, size_t topMaxSize);

    // TODO and what will this construct?
    TagTop() = default;

    std::vector<TagFrequency> tagFrequencies() const;

    std::vector<Sprint> sprintsForTagAt(size_t position) const;

    std::string tagNameAt(size_t position) const;

    size_t topSize() const;

private:
    TagSprints sprintsByTag;
    std::vector<TagFrequency> frequencies;
    size_t numTopTags;
    Tag dummyTag{""};

    void arrangeSprintsByTag(const std::vector<Sprint>& sprints);

    void computeTagFrequencies();

    void orderTagsByDecreasingFrequency();

    void mergeTagsWithLowestFrequencies();

    void buildIndexMap();

    std::vector<Tag> mergeBottomTags();

    std::vector<Tag> findTopTags() const;

    std::vector<Tag> findAllTags() const;

    std::vector<Tag> findBottomTags() const;

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

DayPart dayPart(const TimeSpan& timeSpan);

std::string dayPartName(unsigned dayPart);

std::string dayPartName(DayPart dayPart);

std::string dayPartHours(DayPart dayPart);

std::string dayPartHours(unsigned dayPart);
}


#endif // SPRINTSTATISTICS_H
