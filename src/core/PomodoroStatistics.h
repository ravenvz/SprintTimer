#ifndef POMODORO_POMODOROSTATISTICS_H
#define POMODORO_POMODOROSTATISTICS_H

#include <QDebug>
#include <QtCore/qvector.h>
#include <algorithm>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "TaskScheduler.h"
#include "core/entities/Pomodoro.h"
#include "db_layer/db_service.h"
#include "utils/MathUtils.h"
#include "utils/DateInterval.h"

// TODO refactor this ugly mess

// typedef QHash<QString, QVector<Pomodoro> > PomoHash;
// typedef std::pair<QString, double> TagCount;
// using TagCount = std::pair<QString, double>;
using TagCount = std::pair<std::string, double>;


class TagPomoMap {
public:
    // using PomoHash = QHash<QString, QVector<Pomodoro>>;
    using PomoHash = std::unordered_map<std::string, std::vector<Pomodoro>>;

    TagPomoMap(const std::vector<Pomodoro>& pomodoros, int numTopTagCounts);

    TagPomoMap() = default;

    std::vector<TagCount> getSortedTagCountVector() const;

    std::vector<Pomodoro> getPomodorosWithTag(const std::string& tag) const;

    std::vector<Pomodoro> getPomodorosForTagCount(size_t sliceIndex) const;

    std::string getTag(size_t sliceIndex) const;

private:
    PomoHash map;
    std::unordered_map<std::string, std::vector<Pomodoro>> tagToPomodoroVec;
    std::vector<TagCount> sliceData;
    std::unordered_map<size_t, std::string> sliceIndexMap;
    size_t numTopTagCounts;

    void compute();

    void reduceTailToSum();

    void buildIndexMap();
};

class PomodoroStatItem {

public:
    PomodoroStatItem(const std::vector<Pomodoro>& pomodoros,
                     const TimeInterval& timeInterval);

    PomodoroStatItem(const PomodoroStatItem&) = default;

    ~PomodoroStatItem();

    Distribution<double>* getDailyDistribution() const;

    Distribution<double>* getWeekdayDistribution() const;

    Distribution<double>* getWorkTimeDistribution() const;

    std::vector<Pomodoro> getPomodoros() const;

    std::vector<double> computeDailyDistribution() const;

    std::vector<double> computeWeekdayDistribution() const;

    std::vector<double> computeWorkTimeDistribution();

    std::vector<int> countWeekdays() const;

private:
    const TimeInterval interval;
    const std::vector<Pomodoro> pomodoros;
    Distribution<double>* dailyDistribution;
    Distribution<double>* weekdayDistribution;
    Distribution<double>* workTimeDistribution;
};

#endif // POMODORO_POMODOROSTATISTICS_H
