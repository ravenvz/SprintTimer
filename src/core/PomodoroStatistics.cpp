#include "PomodoroStatistics.h"


TagPomoMap::TagPomoMap(const std::vector<Pomodoro>& pomodoros,
                       int numTopTagCounts)
    : numTopTagCounts(static_cast<size_t>(numTopTagCounts))
{
    for (const Pomodoro& pomo : pomodoros) {
        for (const auto& tag : pomo.tags()) {
            tagToPomodoroVec[tag].push_back(pomo);
        }
    }
    compute();
}


std::vector<TagCount> TagPomoMap::getSortedTagCountVector() const
{
    return sliceData;
}


std::vector<Pomodoro>
TagPomoMap::getPomodorosWithTag(const std::string& tag) const
{
    // May throw out_of_range, but if there is no tag in map, we're screwed
    // anyway so let it crash for now
    return tagToPomodoroVec.at(tag);
}


std::vector<Pomodoro>
TagPomoMap::getPomodorosForTagCount(size_t sliceIndex) const
{
    // May throw out_of_range, but if there is no keys in maps, we're screwed
    // anyway so let it crash for now
    return tagToPomodoroVec.at(sliceIndexMap.at(sliceIndex));
}


std::string TagPomoMap::getTag(size_t sliceIndex) const
{
    // May throw out_of_range, but if there is no key in map, we're screwed
    // anyway so let it crash for now
    return sliceIndexMap.at(sliceIndex);
}


void TagPomoMap::compute()
{
    int total = 0;
    for (const auto& entry : tagToPomodoroVec) {
        sliceData.push_back(std::make_pair(entry.first, entry.second.size()));
        total += entry.second.size();
    }

    // Normalize and sort slice data
    transform(sliceData.begin(),
              sliceData.end(),
              sliceData.begin(),
              [total](const auto& entry) {
                  return std::make_pair(entry.first,
                                        double(entry.second) / total);
              });
    sort(sliceData.begin(),
         sliceData.end(),
         [](const auto& a, const auto& b) { return a.second > b.second; });
    reduceTailToSum();
    buildIndexMap();
}


// TODO should probably be able to find a way to simplify this method
void TagPomoMap::reduceTailToSum()
{
    if (sliceData.size() < numTopTagCounts) {
        return;
    }
    while (sliceData.size() > numTopTagCounts) {
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


void TagPomoMap::buildIndexMap()
{
    for (size_t i = 0; i < sliceData.size(); ++i) {
        sliceIndexMap[i] = sliceData[i].first;
    }
}


PomodoroStatItem::PomodoroStatItem(const std::vector<Pomodoro>& pomodoros,
                                   const TimeInterval& timeInterval)
    : interval(timeInterval)
    , pomodoros(pomodoros)
{
    dailyDistribution = new Distribution<double>{computeDailyDistribution()};
    weekdayDistribution = new Distribution<double>{computeWeekdayDistribution(),
                                                   countWeekdays()};
    workTimeDistribution
        = new Distribution<double>{computeWorkTimeDistribution()};
}


PomodoroStatItem::~PomodoroStatItem()
{
    delete dailyDistribution;
    delete weekdayDistribution;
    delete workTimeDistribution;
}


Distribution<double>* PomodoroStatItem::getDailyDistribution() const
{
    return dailyDistribution;
}


Distribution<double>* PomodoroStatItem::getWeekdayDistribution() const
{
    return weekdayDistribution;
}


Distribution<double>* PomodoroStatItem::getWorkTimeDistribution() const
{
    return workTimeDistribution;
}


std::vector<Pomodoro> PomodoroStatItem::getPomodoros() const
{
    return pomodoros;
}


std::vector<double> PomodoroStatItem::computeDailyDistribution() const
{
    if (pomodoros.empty())
        return std::vector<double>(0, 0);
    std::vector<double> distribution(interval.sizeInDays(), 0);
    // return std::vector<double>(0, 0);
    for (const Pomodoro& pomo : pomodoros) {
        distribution[startDateAbsDiff(interval, pomo.interval())]++;
    }
    return distribution;
}


std::vector<double> PomodoroStatItem::computeWeekdayDistribution() const
{
    std::vector<double> distribution(7, 0);
    if (pomodoros.empty())
        return distribution;
    for (const Pomodoro& pomo : pomodoros) {
        distribution[pomo.startTime().dayOfWeek() - 1]++;
    }
    return distribution;
}


std::vector<double> PomodoroStatItem::computeWorkTimeDistribution()
{
    std::vector<double> distribution(6, 0);
    for (const Pomodoro& pomo : pomodoros) {
        distribution[static_cast<size_t>(pomo.interval().getDayPart())]++;
    }
    return distribution;
}


std::vector<int> PomodoroStatItem::countWeekdays() const
{
    std::vector<int> result(7, 0);
    for (size_t dayNum = 0; dayNum < interval.sizeInDays(); ++dayNum) {
        result[interval.startTime.addDays(static_cast<int>(dayNum)).dayOfWeek()
               - 1]++;
    }
    return result;
}
