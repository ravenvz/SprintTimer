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


TagDistribution::TagDistribution(const std::vector<Sprint>& sprints,
                                 int numTopTags)
    : numTopTags(static_cast<size_t>(numTopTags))
{
    for (const Sprint& sprint : sprints) {
        for (const auto& tag : sprint.tags()) {
            sprintsForTag[tag].push_back(sprint);
        }
    }
    buildDistribution();
}


std::vector<TagCount> TagDistribution::topTagsDistribution() const
{
    return sliceData;
}


std::vector<Sprint> TagDistribution::sprintsWithTag(const Tag &tag) const
{
    // TODO deal with
    // May throw out_of_range, but if there is no tag in map, we're screwed
    // anyway so let it crash for now
    return sprintsForTag.at(tag);
}


std::vector<Sprint> TagDistribution::sprintsForNthTopTag(size_t n) const
{
    // TODO deal with
    // May throw out_of_range, but if there is no keys in maps, we're screwed
    // anyway so let it crash for now
    return sprintsForTag.at(sliceIndexMap.at(n));
}


std::string TagDistribution::getNthTopTagName(size_t n) const
{
    // TODO deal with
    // May throw out_of_range, but if there is no key in map, we're screwed
    // anyway so let it crash for now
    return sliceIndexMap.at(n).name();
}


void TagDistribution::buildDistribution()
{
    int total = 0;
    for (const auto& entry : sprintsForTag) {
        sliceData.push_back(std::make_pair(entry.first, entry.second.size()));
        total += entry.second.size();
    }

    // Normalize and sort slice data
    std::transform(sliceData.begin(),
                   sliceData.end(),
                   sliceData.begin(),
                   [total](const auto& entry) {
                       return std::make_pair(entry.first,
                                             double(entry.second) / total);
                   });
    std::sort(sliceData.begin(),
              sliceData.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    reduceTailToSum();
    buildIndexMap();
}


// TODO should probably be able to find a way to simplify this method
void TagDistribution::reduceTailToSum()
{
    if (sliceData.size() < numTopTags) {
        return;
    }
    while (sliceData.size() > numTopTags) {
        sliceData[sliceData.size() - 2].second += sliceData.back().second;
        sliceData.pop_back();
    }

    const Tag dummyTag{""};

    sliceData.back().first = dummyTag;
    std::vector<Tag> topTagsSet;
    topTagsSet.reserve(sliceData.size());
    std::transform(sliceData.cbegin(),
                   sliceData.cend(),
                   std::back_inserter(topTagsSet),
                   [](const auto& elem) { return elem.first; });

    std::vector<Tag> allTags;
    allTags.reserve(sprintsForTag.size());
    std::transform(sprintsForTag.cbegin(),
                   sprintsForTag.cend(),
                   std::back_inserter(allTags),
                   [](const auto& entry) { return entry.first; });

    std::sort(topTagsSet.begin(), topTagsSet.end());
    std::sort(allTags.begin(), allTags.end());

    std::vector<Tag> otherTags;
    std::set_difference(allTags.begin(),
                        allTags.end(),
                        topTagsSet.begin(),
                        topTagsSet.end(),
                        std::back_inserter(otherTags));

    sprintsForTag.insert(std::make_pair(dummyTag, std::vector<Sprint>()));
    for (const auto& tag : otherTags) {
        sprintsForTag[dummyTag].insert(sprintsForTag[dummyTag].end(),
                                          sprintsForTag[tag].begin(),
                                          sprintsForTag[tag].end());
    }
}


void TagDistribution::buildIndexMap()
{
    for (size_t i = 0; i < sliceData.size(); ++i) {
        sliceIndexMap[i] = sliceData[i].first;
    }
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
        distribution[static_cast<size_t>(DayPart::dayPart(sprint.timeSpan()))]++;
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

