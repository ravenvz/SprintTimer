#ifndef POMODORO_POMODOROSTATISTICS_H
#define POMODORO_POMODOROSTATISTICS_H

#include "core/entities/Pomodoro.h"
#include "db_layer/db_service.h"
#include "utils/DateInterval.h"
#include "utils/MathUtils.h"
#include <algorithm>
#include <iostream>
#include <unordered_map>
#include <vector>

// TODO refactor this ugly mess

using TagCount = std::pair<std::string, double>;


/* Datastructure that stores number of completed pomodoros for each tag. */
class TagDistribution {
public:
    using TagPomoMap = std::unordered_map<std::string, std::vector<Pomodoro>>;

    TagDistribution(const std::vector<Pomodoro>& pomodoros, int numTopTags);

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
    std::vector<Pomodoro> pomodorosWithTag(const std::string& tag) const;

    /* Return vector of pomodoros for n-th top tag. */
    std::vector<Pomodoro> pomodorosForNthTopTag(size_t n) const;

    /* Return n-th top tag name. */
    std::string getNthTopTagName(size_t n) const;

private:
    TagPomoMap map;
    std::unordered_map<std::string, std::vector<Pomodoro>> tagToPomodoroVec;
    std::vector<TagCount> sliceData;
    std::unordered_map<size_t, std::string> sliceIndexMap;
    size_t numTopTags;

    void buildDistribution();

    void reduceTailToSum();

    void buildIndexMap();
};

class PomodoroStatItem {

public:
    PomodoroStatItem(const std::vector<Pomodoro>& pomodoros,
                     const TimeSpan& timeInterval);

    PomodoroStatItem(const PomodoroStatItem&) = default;

    ~PomodoroStatItem() = default;

    Distribution<double> dailyDistribution() const;

    Distribution<double> weekdayDistribution() const;

    Distribution<double> worktimeDistribution() const;

    std::vector<Pomodoro> pomodoros() const;

private:
    const TimeSpan timeSpan;
    const std::vector<Pomodoro> pomos;

    std::vector<double> computeDailyDistribution() const;

    std::vector<double> computeWeekdayDistribution() const;

    std::vector<double> computeWorkTimeDistribution() const;

    std::vector<int> countWeekdays() const;
};

#endif // POMODORO_POMODOROSTATISTICS_H
