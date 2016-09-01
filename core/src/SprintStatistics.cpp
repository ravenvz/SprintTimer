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
#include "core/SprintStatistics.h"
#include <numeric>


TagTop::TagTop(const std::vector<Sprint>& sprints, size_t topMaxSize)
    : numTopTags{topMaxSize}
{
    arrangeSprintsByTag(sprints);
    computeTagFrequencies();
    orderTagsByDecreasingFrequency();
    mergeTagsWithLowestFrequencies();
}


void TagTop::arrangeSprintsByTag(const std::vector<Sprint>& sprints)
{
    for (const Sprint& sprint : sprints)
        for (const auto& tag : sprint.tags())
            sprintsByTag[tag].push_back(sprint);
}


void TagTop::computeTagFrequencies()
{
    int total = std::accumulate(
        sprintsByTag.cbegin(),
        sprintsByTag.cend(),
        0,
        [](int aux, const auto& entry) { return aux + entry.second.size(); });

    for (const auto& entry : sprintsByTag) {
        frequencies.push_back(
            {entry.first, entry.second.size() / double(total)});
    }
}


void TagTop::orderTagsByDecreasingFrequency()
{
    std::sort(frequencies.begin(),
              frequencies.end(),
              [](const auto& lhs, const auto& rhs) {
                  return lhs.second > rhs.second;
              });
}


void TagTop::mergeTagsWithLowestFrequencies()
{
    if (frequencies.size() < numTopTags) {
        return;
    }

    auto otherTags = mergeBottomTags();

    for (const auto& tag : otherTags) {
        sprintsByTag[dummyTag].insert(sprintsByTag[dummyTag].end(),
                                      sprintsByTag[tag].begin(),
                                      sprintsByTag[tag].end());
    }
}


std::vector<Tag> TagTop::mergeBottomTags()
{
    while (frequencies.size() > numTopTags) {
        frequencies[frequencies.size() - 2].second += frequencies.back().second;
        frequencies.pop_back();
    }
    frequencies.back().first = dummyTag;
    sprintsByTag.insert(std::make_pair(dummyTag, std::vector<Sprint>()));
    return findBottomTags();
}


std::vector<Tag> TagTop::findBottomTags() const
{
    std::vector<Tag> topTags = findTopTags();
    std::vector<Tag> allTags = findAllTags();
    std::sort(topTags.begin(), topTags.end());
    std::sort(allTags.begin(), allTags.end());

    std::vector<Tag> bottomTags;
    std::set_difference(allTags.begin(),
                        allTags.end(),
                        topTags.begin(),
                        topTags.end(),
                        std::back_inserter(bottomTags));

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


std::vector<TagTop::TagFrequency> TagTop::tagFrequencies() const { return frequencies; }


std::vector<Sprint> TagTop::sprintsForTagAt(size_t position) const
{
    return sprintsByTag.at(tagNameAt(position));
}


std::string TagTop::tagNameAt(size_t position) const
{
    return frequencies.at(position).first.name();
}


size_t TagTop::topSize() const {
    return frequencies.size();
}


SprintStatItem::SprintStatItem(const std::vector<Sprint>& sprints,
                               const TimeSpan& timeInterval)
    : timeSpan(timeInterval)
    , mSprints(sprints)
{
}


Distribution<double> SprintStatItem::dailyDistribution() const
{
    return Distribution<double>{computeDailyDistribution()};
}


Distribution<double> SprintStatItem::weekdayDistribution() const
{
    return Distribution<double>{computeWeekdayDistribution(), countWeekdays()};
}


Distribution<double> SprintStatItem::worktimeDistribution() const
{
    return Distribution<double>{computeWorkTimeDistribution()};
}


std::vector<Sprint> SprintStatItem::sprints() const { return mSprints; }


std::vector<double> SprintStatItem::computeDailyDistribution() const
{
    if (mSprints.empty())
        return std::vector<double>(0, 0);
    std::vector<double> distribution(timeSpan.sizeInDays(), 0);
    for (const Sprint& sprint : mSprints) {
        distribution[startDateAbsDiff(timeSpan, sprint.timeSpan())]++;
    }
    return distribution;
}


std::vector<double> SprintStatItem::computeWeekdayDistribution() const
{
    std::vector<double> distribution(7, 0);
    if (mSprints.empty())
        return distribution;
    for (const Sprint& sprint : mSprints) {
        distribution[sprint.startTime().dayOfWeek() - 1]++;
    }
    return distribution;
}


std::vector<double> SprintStatItem::computeWorkTimeDistribution() const
{
    std::vector<double> distribution(DayPart::numParts, 0);
    for (const Sprint& sprint : mSprints) {
        distribution[static_cast<size_t>(
            DayPart::dayPart(sprint.timeSpan()))]++;
    }
    return distribution;
}


std::vector<int> SprintStatItem::countWeekdays() const
{
    std::vector<int> result(7, 0);
    for (size_t dayNum = 0; dayNum < timeSpan.sizeInDays(); ++dayNum) {
        result[timeSpan.startTime.addDays(static_cast<int>(dayNum)).dayOfWeek()
               - 1]++;
    }
    return result;
}

namespace DayPart {


DayPart dayPart(const TimeSpan& timeSpan)
{
    auto hour = timeSpan.startTime.hour();

    if (22 < hour || hour <= 2) {
        return DayPart::Midnight;
    }
    else if (2 < hour && hour <= 6) {
        return DayPart::Night;
    }
    else if (6 < hour && hour <= 10) {
        return DayPart::Morning;
    }
    else if (10 < hour && hour <= 14) {
        return DayPart::Noon;
    }
    else if (14 < hour && hour <= 18) {
        return DayPart::Afternoon;
    }
    else {
        return DayPart::Evening;
    }
}

std::string dayPartName(unsigned dayPart)
{
    return dayPartName(static_cast<DayPart>(dayPart));
}

std::string dayPartName(DayPart dayPart)
{
    switch (dayPart) {
    case DayPart::Midnight:
        return "Midnight";
    case DayPart::Night:
        return "Night";
    case DayPart::Morning:
        return "Morning";
    case DayPart::Noon:
        return "Noon";
    case DayPart::Afternoon:
        return "Afternoon";
    case DayPart::Evening:
        return "Evening";
    }
    return "Invalid";
}

std::string dayPartHours(DayPart dayPart)
{
    switch (dayPart) {
    case DayPart::Midnight:
        return "22:00 - 2:00";
    case DayPart::Night:
        return "2:00 - 6:00";
    case DayPart::Morning:
        return "6:00 - 10:00";
    case DayPart::Noon:
        return "10:00 - 14:00";
    case DayPart::Afternoon:
        return "14:00 - 18:00";
    case DayPart::Evening:
        return "18:00 - 22:00";
    }
    return "Invalid";
}

std::string dayPartHours(unsigned dayPart)
{
    return dayPartHours(static_cast<DayPart>(dayPart));
}

} // namespace DayPart
