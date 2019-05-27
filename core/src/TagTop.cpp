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

#include "core/TagTop.h"
#include "core/SprintStatistics.h"
#include <numeric>

namespace sprint_timer {

using namespace entities;

TagTop::TagTop(const std::vector<Sprint>& sprints, size_t topMaxSize)
    : numTopTags{topMaxSize}
{
    arrangeSprintsByTag(sprints);
    computeTagFrequencies();
    orderTagsByDecreasingFrequency();
    mergeTagsWithLowestFrequencies();
}

TagTop::TagTop() {}

void TagTop::arrangeSprintsByTag(const std::vector<Sprint>& sprints)
{
    for (const Sprint& sprint : sprints)
        for (const auto& tag : sprint.tags())
            sprintsByTag[tag].push_back(sprint);
}

void TagTop::computeTagFrequencies()
{
    const auto total = accumulate(
        sprintsByTag.cbegin(),
        sprintsByTag.cend(),
        0uLL,
        [](auto aux, const auto& entry) { return aux + entry.second.size(); });

    for (const auto& entry : sprintsByTag) {
        frequencies.push_back(
            {entry.first, entry.second.size() / double(total)});
    }
}

void TagTop::orderTagsByDecreasingFrequency()
{
    const auto limit = std::min(numTopTags, frequencies.size());
    std::partial_sort(frequencies.begin(),
                      frequencies.begin() + static_cast<long>(limit),
                      frequencies.end(),
                      [](const auto& lhs, const auto& rhs) {
                          return lhs.second > rhs.second;
                      });
}

void TagTop::mergeTagsWithLowestFrequencies()
{
    if (frequencies.size() < numTopTags)
        return;

    const auto otherTags = mergeBottomTags();

    for (const auto& tag : otherTags)
        sprintsByTag[dummyTag].insert(sprintsByTag[dummyTag].end(),
                                      sprintsByTag[tag].begin(),
                                      sprintsByTag[tag].end());
}

std::vector<Tag> TagTop::mergeBottomTags()
{
    while (frequencies.size() > numTopTags) {
        frequencies[frequencies.size() - 2].second += frequencies.back().second;
        frequencies.pop_back();
    }
    frequencies.back().first = dummyTag;
    sprintsByTag.insert(make_pair(dummyTag, std::vector<Sprint>()));
    return findBottomTags();
}

std::vector<Tag> TagTop::findBottomTags() const
{
    std::vector<Tag> topTags = findTopTags();
    std::vector<Tag> allTags = findAllTags();
    std::sort(topTags.begin(), topTags.end());
    std::sort(allTags.begin(), allTags.end());

    std::vector<Tag> bottomTags;
    set_difference(allTags.begin(),
                   allTags.end(),
                   topTags.begin(),
                   topTags.end(),
                   back_inserter(bottomTags));

    return bottomTags;
}

std::vector<Tag> TagTop::findTopTags() const
{
    std::vector<Tag> topTagsSet;
    topTagsSet.reserve(frequencies.size());
    transform(frequencies.cbegin(),
              frequencies.cend(),
              back_inserter(topTagsSet),
              [](const auto& elem) { return elem.first; });
    return topTagsSet;
}

std::vector<Tag> TagTop::findAllTags() const
{
    std::vector<Tag> allTags;
    allTags.reserve(sprintsByTag.size());
    transform(sprintsByTag.cbegin(),
              sprintsByTag.cend(),
              back_inserter(allTags),
              [](const auto& entry) { return entry.first; });

    return allTags;
}

std::vector<TagTop::TagFrequency> TagTop::tagFrequencies() const
{
    return frequencies;
}

std::vector<Sprint> TagTop::sprintsForTagAt(size_t position) const
{
    return sprintsByTag.at(tagNameAt(position));
}

std::string TagTop::tagNameAt(size_t position) const
{
    return frequencies.at(position).first.name();
}

size_t TagTop::topSize() const { return frequencies.size(); }

} // namespace sprint_timer
