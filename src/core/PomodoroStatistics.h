#ifndef POMODORO_POMODOROSTATISTICS_H
#define POMODORO_POMODOROSTATISTICS_H

#include <QtCore/qvector.h>
#include "core/entities/Pomodoro.h"
#include <QDebug>
#include "utils/MathUtils.h"
#include "TaskScheduler.h"
#include "db_layer/db_service.h"
#include <iostream>
#include <algorithm>

// TODO refactor this ugly mess

// typedef QHash<QString, QVector<Pomodoro> > PomoHash;
// typedef std::pair<QString, double> Slice;
using Slice = std::pair<QString, double>;

class TagPomoMap {
public:
    using PomoHash = QHash<QString, QVector<Pomodoro>>;

    TagPomoMap(const QVector<Pomodoro>& pomodoros, int numTopSlices);

    TagPomoMap() = default;

    QVector<Slice> getSortedSliceVector() const;

    QVector<Pomodoro> getPomodorosWithTag(QString tag) const;

    QVector<Pomodoro> getPomodorosForSlice(int sliceIndex) const;

    QString getTag(int sliceIndex) const;

private:
    PomoHash map;
    QHash<QString, QVector<Pomodoro>> tagToPomodoroVec;
    QVector<Slice> sliceData;
    QHash<int, QString> sliceIndexMap;
    int numTopSlices;

    void compute();

    void reduceTailToSum();

    void buildIndexMap();
};

class PomodoroStatItem {

public:
    PomodoroStatItem(const QVector<Pomodoro>& pomodoros,
                     const DateInterval& dateInterval);

    PomodoroStatItem(const PomodoroStatItem&) = default;

    ~PomodoroStatItem();

    Distribution<double>* getDailyDistribution() const;

    Distribution<double>* getWeekdayDistribution() const;

    Distribution<double>* getWorkTimeDistribution() const;

    QVector<Pomodoro> getPomodoros() const;

    QVector<double> computeDailyDistribution() const;

    QVector<double> computeWeekdayDistribution() const;

    QVector<double> computeWorkTimeDistribution();

    QVector<int> countWeekdays() const;

private:
    const DateInterval interval;
    const QVector<Pomodoro> pomodoros;
    Distribution<double>* dailyDistribution;
    Distribution<double>* weekdayDistribution;
    Distribution<double>* workTimeDistribution;
};

#endif // POMODORO_POMODOROSTATISTICS_H
