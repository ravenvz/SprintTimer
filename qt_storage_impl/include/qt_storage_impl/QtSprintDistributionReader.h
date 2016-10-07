/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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
#ifndef QTSPRINTDISTRIBUTIONREADER_H_TS4GUJR3
#define QTSPRINTDISTRIBUTIONREADER_H_TS4GUJR3

#include "core/ISprintDistributionReader.h"
#include "qt_storage_impl/DBService.h"
#include <QObject>


class DistributionReaderBase : public QObject,
                               public ISprintDistributionReader {

    Q_OBJECT

public:
    virtual ~DistributionReaderBase() = default;

    virtual void requestDistribution(const TimeSpan& timeSpan,
                                     Handler handler) final;

protected:
    DBService& dbService;
    size_t distributionSize;
    std::list<Handler> handler_queue;
    QDate startDate;
    QDate endDate;
    std::function<bool(const QDate&, const QDate&)> equalityFunc;
    std::function<QDate(const QDate&)> incrementFunc;
    long long mQueryId{-1};
    bool invalidQueryId(long long int queryId) const;

    DistributionReaderBase(
        DBService& dbService,
        size_t distributionSize,
        std::function<bool(const QDate&, const QDate&)> compareFunc,
        std::function<QDate(const QDate&)> incrementFunc);

    void executeCallback(std::vector<int>&& sprintCount);

    template <class EqualityCriteria, class DateIncrementer>
    std::vector<int> fillDateGaps(const std::vector<QSqlRecord>& records,
                                  EqualityCriteria comp,
                                  DateIncrementer incrementer)
    {

        std::vector<int> sprintCount(distributionSize, 0);

        QDate current = startDate;
        auto it = cbegin(records);
        for (auto& elem : sprintCount) {
            if (it == cend(records))
                break;
            if (comp(current, it->value(1).toDate())) {
                elem = it->value(0).toInt();
                ++it;
            }
            current = incrementer(current);
        }

        return sprintCount;
    }

private slots:
    virtual void onResultsReceived(long long queryId,
                                   const std::vector<QSqlRecord>& records);
};

class QtSprintDailyDistributionReader : public DistributionReaderBase {
public:
    QtSprintDailyDistributionReader(DBService& dbService, size_t numBins);
};

class QtSprintWeeklyDistributionReader : public DistributionReaderBase {
public:
    QtSprintWeeklyDistributionReader(DBService& dbService, size_t numBins);
};

class QtSprintMonthlyDistributionReader : public DistributionReaderBase {
public:
    QtSprintMonthlyDistributionReader(DBService& dbService, size_t numBins);
};

#endif /* end of include guard: QTSPRINTDISTRIBUTIONREADER_H_TS4GUJR3 */
