/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include <core/IConfig.h>
#include <queue>

namespace sprint_timer::storage::qt_storage_impl {

class DistributionReaderBase : public QObject,
                               public ISprintDistributionReader {

    Q_OBJECT

public:
    void requestDistribution(const dw::DateRange& dateRange,
                             Handler handler) override;

protected:
    struct Context {
        Handler handler;
        QDate startDate;
    };
    DBService& dbService;
    size_t distributionSize;
    std::queue<Context> contextQueue;
    qint64 mQueryId{-1};

    bool invalidQueryId(qint64 queryId) const;

    DistributionReaderBase(DBService& dbService, size_t distributionSize);

    void executeCallback(std::vector<int>&& sprintCount);

    virtual QDate nextExpectedDate(const QDate& referenceDate) const = 0;

    virtual QDate normalizeDate(const QDate& date) const;

    virtual bool compareDate(const QDate& expected,
                             const QDate& probeDate) const;

private:
    std::vector<int> fillDateGaps(const std::vector<QSqlRecord>& records) const;

private slots:
    virtual void onResultsReceived(qint64 queryId,
                                   const std::vector<QSqlRecord>& records);
};

class QtSprintDailyDistributionReader : public DistributionReaderBase {
public:
    QtSprintDailyDistributionReader(DBService& dbService_, size_t numBins);

private:
    QDate nextExpectedDate(const QDate& referenceDate) const override;
};


class QtSprintDistReaderMondayFirst : public DistributionReaderBase {
public:
    QtSprintDistReaderMondayFirst(DBService& dbService_, size_t numBins);

private:
    QDate nextExpectedDate(const QDate& referenceDate) const override;

    QDate normalizeDate(const QDate& date) const override;

    bool compareDate(const QDate& expected,
                     const QDate& probeDate) const override;
};

class QtSprintDistReaderSundayFirst : public DistributionReaderBase {
public:
    QtSprintDistReaderSundayFirst(DBService& dbService_, size_t numBins);

private:
    QDate nextExpectedDate(const QDate& referenceDate) const override;

    QDate normalizeDate(const QDate& date) const override;

    bool compareDate(const QDate& expected,
                     const QDate& probeDate) const override;
};

class QtSprintMonthlyDistributionReader : public DistributionReaderBase {
public:
    QtSprintMonthlyDistributionReader(DBService& dbService_, size_t numBins);

private:
    QDate nextExpectedDate(const QDate& referenceDate) const override;

    bool compareDate(const QDate& expected,
                     const QDate& probeDate) const override;
};

} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: QTSPRINTDISTRIBUTIONREADER_H_TS4GUJR3 */
