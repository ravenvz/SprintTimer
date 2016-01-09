#ifndef POMODORO_POMODOROSTATISTICS_H
#define POMODORO_POMODOROSTATISTICS_H


#include <QtCore/qvector.h>
#include "core/entities/Pomodoro.h"
#include <QDebug>
#include "utils/MathUtils.h"
#include "TaskScheduler.h"
#include "db_layer/db_helper.h"
#include <iostream>
#include <algorithm>


// TODO refactor this ugly mess

// typedef QHash<QString, QVector<Pomodoro> > PomoHash;
// typedef std::pair<QString, double> Slice;
using Slice = std::pair<QString, double>;


class TagPomoMap
{
public:
    using PomoHash = QHash<QString, QVector<Pomodoro> >;

    TagPomoMap(const QVector<Pomodoro>& pomodoros, int numTopSlices) :
        numTopSlices(numTopSlices)
    {
        for (const Pomodoro& pomo : pomodoros) {
            for (const auto& tag : pomo.getTags()) {
                tagToPomodoroVec[tag] << pomo;
            }
        }
        compute();
    }

    TagPomoMap() {
    }

    QVector<Slice> getSortedSliceVector() const {
        return sliceData;
    }

    QVector<Pomodoro> getPomodorosWithTag(QString tag) const {
        return tagToPomodoroVec.value(tag, QVector<Pomodoro> ());
    }

    QVector<Pomodoro> getPomodorosForSlice(int sliceIndex) const {
        return tagToPomodoroVec[sliceIndexMap[sliceIndex]];
    }

    QString getTag(int sliceIndex) const {
        return sliceIndexMap.value(sliceIndex, "");
    }

private:
    PomoHash map;
    QHash<QString, QVector<Pomodoro> > tagToPomodoroVec;
    QVector<Slice> sliceData;
    QHash<int, QString> sliceIndexMap;
    int numTopSlices;

    void compute() {
        unsigned total = 0;
        for (auto it = tagToPomodoroVec.cbegin(); it != tagToPomodoroVec.cend(); ++it) {
            sliceData.append(std::make_pair(it.key(), it.value().size()));
            total += it.value().size();
        }

        // Normalize and sort slice data
        transform(sliceData.begin(), sliceData.end(), sliceData.begin(), [total](const auto& entry) {
                return std::make_pair(entry.first, double(entry.second) / total);
            });
        sort(sliceData.begin(), sliceData.end(), [](const auto& a, const auto& b) {
               return a.second > b.second;
           });
        reduceTailToSum();
        buildIndexMap();
    }

    void reduceTailToSum() {
        if (sliceData.size() < numTopSlices) {
            return;
        }
        while (sliceData.size() > numTopSlices) {
            sliceData[sliceData.size() - 2].second += sliceData.last().second;
            sliceData.pop_back();
        }
        sliceData.back().first = "";
        QSet<QString> topTagsSet;
        for (const auto& data : sliceData) {
            topTagsSet.insert(data.first);
        }
        QSet<QString> allTags = QSet<QString>::fromList(tagToPomodoroVec.keys());
        QSet<QString> otherTags = allTags - topTagsSet;

        QSet<QString>::const_iterator it;
        for (it = otherTags.constBegin(); it != otherTags.constEnd(); ++it) {
            tagToPomodoroVec[""] << tagToPomodoroVec.value(*it);
        }
    }

    void buildIndexMap() {
        for (int i = 0; i < sliceData.size(); ++i) {
            sliceIndexMap[i] = sliceData[i].first;
        }
    }
};


class PomodoroStatItem
{

public:

    PomodoroStatItem(const QVector<Pomodoro>& pomodoros, const DateInterval& dateInterval) :
        interval(dateInterval),
        pomodoros(pomodoros)
    {
        dailyDistribution = new Distribution<double> {computeDailyDistribution()};
        weekdayDistribution = new Distribution<double> {computeWeekdayDistribution(), countWeekdays()};
        workTimeDistribution = new Distribution<double> {computeWorkTimeDistribution()};
    }

    PomodoroStatItem(const PomodoroStatItem&) = default;

    ~PomodoroStatItem() {
        delete dailyDistribution;
        delete weekdayDistribution;
        delete workTimeDistribution;
    }

    Distribution<double>* getDailyDistribution() const {
        return dailyDistribution;
    }

    Distribution<double>* getWeekdayDistribution() const {
        return weekdayDistribution;
    }

    Distribution<double>* getWorkTimeDistribution() const {
        return workTimeDistribution;
    }

    QVector<Pomodoro> getPomodoros() const {
        return pomodoros;
    }

    QVector<double> computeDailyDistribution() const {
        if (pomodoros.empty()) return QVector<double> (0, 0);
        QVector<double> distribution (interval.sizeInDays(), 0);
        for (const Pomodoro& pomo : pomodoros) {
            distribution[interval.startDate.daysTo(pomo.getStartTime().date())]++;
        }
        return distribution;
    }

    QVector<double> computeWeekdayDistribution() const {
        QVector<double> distribution (7, 0);
        if (pomodoros.empty()) return distribution;
        for (const Pomodoro& pomo : pomodoros) {
            distribution[pomo.getStartTime().date().dayOfWeek() - 1]++;
        }
        return distribution;
    }

    QVector<double> computeWorkTimeDistribution() {
        QVector<double> distribution (6, 0);
        for (const Pomodoro& pomo : pomodoros) {
            TimeInterval currentInterval = TimeInterval {pomo.getStartTime(), pomo.getFinishTime()};
            distribution[static_cast<int>(currentInterval.getDayPart())]++;
        }
        return distribution;
    }

    QVector<int> countWeekdays() const {
        QVector<int> result (7, 0);
        for (int weekdayNum = 0; weekdayNum <= interval.startDate.daysTo(interval.endDate); ++weekdayNum) {
            result[interval.startDate.addDays(weekdayNum).dayOfWeek() - 1]++;
        }
        return result;
    }

private:
    const DateInterval interval;
    const QVector<Pomodoro> pomodoros;
    Distribution<double>* dailyDistribution;
    Distribution<double>* weekdayDistribution;
    Distribution<double>* workTimeDistribution;
};


#endif //POMODORO_POMODOROSTATISTICS_H
