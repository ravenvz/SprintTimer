#include "PomodoroStatistics.h"


TagDistribution::TagDistribution(const std::vector<Pomodoro>& pomodoros,
                                 int numTopTags)
    : numTopTags(static_cast<size_t>(numTopTags))
{
    for (const Pomodoro& pomo : pomodoros) {
        for (const auto& tag : pomo.tags()) {
            tagToPomodoroVec[tag].push_back(pomo);
        }
    }
    buildDistribution();
}


std::vector<TagCount> TagDistribution::topTagsDistribution() const
{
    return sliceData;
}


std::vector<Pomodoro>
TagDistribution::pomodorosWithTag(const std::string& tag) const
{
    // May throw out_of_range, but if there is no tag in map, we're screwed
    // anyway so let it crash for now
    return tagToPomodoroVec.at(tag);
}


std::vector<Pomodoro> TagDistribution::pomodorosForNthTopTag(size_t n) const
{
    // May throw out_of_range, but if there is no keys in maps, we're screwed
    // anyway so let it crash for now
    return tagToPomodoroVec.at(sliceIndexMap.at(n));
}


std::string TagDistribution::getNthTopTagName(size_t n) const
{
    // May throw out_of_range, but if there is no key in map, we're screwed
    // anyway so let it crash for now
    return sliceIndexMap.at(n);
}


void TagDistribution::buildDistribution()
{
    int total = 0;
    for (const auto& entry : tagToPomodoroVec) {
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
    sliceData.back().first = "";
    std::vector<std::string> topTagsSet;
    topTagsSet.reserve(sliceData.size());
    std::transform(sliceData.cbegin(),
                   sliceData.cend(),
                   std::back_inserter(topTagsSet),
                   [](const auto& elem) { return elem.first; });

    std::vector<std::string> allTags;
    allTags.reserve(tagToPomodoroVec.size());
    std::transform(tagToPomodoroVec.cbegin(),
                   tagToPomodoroVec.cend(),
                   std::back_inserter(allTags),
                   [](const auto& entry) { return entry.first; });

    std::sort(topTagsSet.begin(), topTagsSet.end());
    std::sort(allTags.begin(), allTags.end());

    std::vector<std::string> otherTags;
    std::set_difference(allTags.begin(),
                        allTags.end(),
                        topTagsSet.begin(),
                        topTagsSet.end(),
                        std::back_inserter(otherTags));

    tagToPomodoroVec.insert(std::make_pair("", std::vector<Pomodoro>()));
    for (const auto& tag : otherTags) {
        tagToPomodoroVec[""].insert(tagToPomodoroVec[""].end(),
                                    tagToPomodoroVec[tag].begin(),
                                    tagToPomodoroVec[tag].end());
    }
}


void TagDistribution::buildIndexMap()
{
    for (size_t i = 0; i < sliceData.size(); ++i) {
        sliceIndexMap[i] = sliceData[i].first;
    }
}


PomodoroStatItem::PomodoroStatItem(const std::vector<Pomodoro>& pomodoros,
                                   const TimeSpan& timeInterval)
    : timeSpan(timeInterval)
    , pomos(pomodoros)
{
}


Distribution<double> PomodoroStatItem::dailyDistribution() const
{
    return Distribution<double>{computeDailyDistribution()};
}


Distribution<double> PomodoroStatItem::weekdayDistribution() const
{
    return Distribution<double>{computeWeekdayDistribution(), countWeekdays()};
}


Distribution<double> PomodoroStatItem::worktimeDistribution() const
{
    return Distribution<double>{computeWorkTimeDistribution()};
}


std::vector<Pomodoro> PomodoroStatItem::pomodoros() const { return pomos; }


std::vector<double> PomodoroStatItem::computeDailyDistribution() const
{
    if (pomos.empty())
        return std::vector<double>(0, 0);
    std::vector<double> distribution(timeSpan.sizeInDays(), 0);
    for (const Pomodoro& pomo : pomos) {
        distribution[startDateAbsDiff(timeSpan, pomo.timeSpan())]++;
    }
    return distribution;
}


std::vector<double> PomodoroStatItem::computeWeekdayDistribution() const
{
    std::vector<double> distribution(7, 0);
    if (pomos.empty())
        return distribution;
    for (const Pomodoro& pomo : pomos) {
        distribution[pomo.startTime().dayOfWeek() - 1]++;
    }
    return distribution;
}


std::vector<double> PomodoroStatItem::computeWorkTimeDistribution() const
{
    std::vector<double> distribution(6, 0);
    for (const Pomodoro& pomo : pomos) {
        distribution[static_cast<size_t>(pomo.timeSpan().getDayPart())]++;
    }
    return distribution;
}


std::vector<int> PomodoroStatItem::countWeekdays() const
{
    std::vector<int> result(7, 0);
    for (size_t dayNum = 0; dayNum < timeSpan.sizeInDays(); ++dayNum) {
        result[timeSpan.startTime.addDays(static_cast<int>(dayNum)).dayOfWeek()
               - 1]++;
    }
    return result;
}
