/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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

#ifndef SPRINT_TIMER_TAGTOP_H
#define SPRINT_TIMER_TAGTOP_H

#include <iostream>
#include <unordered_map>
#include "Distribution.h"
#include "core/entities/Sprint.h"


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

    TagTop();

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

#endif //SPRINT_TIMER_TAGTOP_H
