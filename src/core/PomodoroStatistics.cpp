#include "PomodoroStatistics.h"


TagPomoMap::TagPomoMap(const QVector<Pomodoro>& pomodoros, int numTopSlices)
    : numTopSlices(numTopSlices)
{
    for (const Pomodoro& pomo : pomodoros) {
        const auto& tags = pomo.tags();
        for (const auto& tag : tags) {
            tagToPomodoroVec[tag] << pomo;
        }
    }
    compute();
}


QVector<Slice> TagPomoMap::getSortedSliceVector() const { return sliceData; }


QVector<Pomodoro> TagPomoMap::getPomodorosWithTag(QString tag) const
{
    return tagToPomodoroVec.value(tag, QVector<Pomodoro>());
}


QVector<Pomodoro> TagPomoMap::getPomodorosForSlice(int sliceIndex) const
{
    return tagToPomodoroVec[sliceIndexMap[sliceIndex]];
}


QString TagPomoMap::getTag(int sliceIndex) const
{
    return sliceIndexMap.value(sliceIndex, "");
}


void TagPomoMap::compute()
{
    int total = 0;
    for (auto it = tagToPomodoroVec.cbegin(); it != tagToPomodoroVec.cend();
         ++it) {
        sliceData.append(std::make_pair(it.key(), it.value().size()));
        total += it.value().size();
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


void TagPomoMap::reduceTailToSum()
{
    if (sliceData.size() < numTopSlices) {
        return;
    }
    while (sliceData.size() > numTopSlices) {
        sliceData[sliceData.size() - 2].second += sliceData.last().second;
        sliceData.pop_back();
    }
    sliceData.back().first = "";
    QSet<QString> topTagsSet;
    const auto& sliceDataRef = sliceData;
    for (const auto& data : sliceDataRef) {
        topTagsSet.insert(data.first);
    }
    QSet<QString> allTags = QSet<QString>::fromList(tagToPomodoroVec.keys());
    QSet<QString> otherTags = allTags - topTagsSet;

    QSet<QString>::const_iterator it;
    for (it = otherTags.constBegin(); it != otherTags.constEnd(); ++it) {
        tagToPomodoroVec[""] << tagToPomodoroVec.value(*it);
    }
}


void TagPomoMap::buildIndexMap()
{
    for (int i = 0; i < sliceData.size(); ++i) {
        sliceIndexMap[i] = sliceData[i].first;
    }
}


PomodoroStatItem::PomodoroStatItem(const QVector<Pomodoro>& pomodoros,
                                   const DateInterval& dateInterval)
    : interval(dateInterval)
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


QVector<Pomodoro> PomodoroStatItem::getPomodoros() const { return pomodoros; }


QVector<double> PomodoroStatItem::computeDailyDistribution() const
{
    if (pomodoros.empty())
        return QVector<double>(0, 0);
    QVector<double> distribution(interval.sizeInDays(), 0);
    for (const Pomodoro& pomo : pomodoros) {
        // TODO when switched to std::vector, remove cast
        distribution[static_cast<int>(
            interval.startDate.daysTo(pomo.startTime().date()))]++;
    }
    return distribution;
}


QVector<double> PomodoroStatItem::computeWeekdayDistribution() const
{
    QVector<double> distribution(7, 0);
    if (pomodoros.empty())
        return distribution;
    const auto& pomodorosRef = pomodoros;
    for (const Pomodoro& pomo : pomodorosRef) {
        distribution[pomo.startTime().date().dayOfWeek() - 1]++;
    }
    return distribution;
}


QVector<double> PomodoroStatItem::computeWorkTimeDistribution()
{
    QVector<double> distribution(6, 0);
    const auto& pomodorosRef = pomodoros;
    for (const Pomodoro& pomo : pomodorosRef) {
        distribution[static_cast<int>(pomo.interval().getDayPart())]++;
    }
    return distribution;
}


QVector<int> PomodoroStatItem::countWeekdays() const
{
    QVector<int> result(7, 0);
    for (int weekdayNum = 0;
         weekdayNum <= interval.startDate.daysTo(interval.endDate);
         ++weekdayNum) {
        result[interval.startDate.addDays(weekdayNum).dayOfWeek() - 1]++;
    }
    return result;
}
