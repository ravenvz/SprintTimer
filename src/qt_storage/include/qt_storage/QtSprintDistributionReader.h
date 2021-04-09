/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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

#include "core/SprintDistributionReader.h"
#include <QDate>
#include <QSqlQuery>
#include <core/IConfig.h>

namespace sprint_timer::storage::qt_storage {

class DistributionReaderBase : public SprintDistributionReader {
public:
    DistributionReaderBase(size_t distributionSize);

    DistributionReaderBase(DistributionReaderBase&&) = delete;
    DistributionReaderBase& operator=(DistributionReaderBase&&) = delete;

    DistributionReaderBase(const DistributionReaderBase&) = delete;
    DistributionReaderBase& operator=(const DistributionReaderBase&) = delete;

    std::vector<int>
    sprintDistribution(const dw::DateRange& dateRange) override;

protected:
    QSqlQuery rangeQuery;

    virtual QDate nextExpectedDate(const QDate& referenceDate) const = 0;

    virtual QDate normalizeDate(const QDate& date) const;

    virtual bool compareDate(const QDate& expected,
                             const QDate& probeDate) const;

    std::vector<int> zeroFilledDistribution(
        const QDate& startDate,
        const std::vector<std::pair<QDate, int>>& query) const;

private:
    size_t distributionSize;
};

class QtSprintDailyDistributionReader : public DistributionReaderBase {
public:
    QtSprintDailyDistributionReader(const QString& connectionName_,
                                    size_t numBins);

private:
    QDate nextExpectedDate(const QDate& referenceDate) const override;
};

class QtSprintDistReaderMondayFirst : public DistributionReaderBase {
public:
    QtSprintDistReaderMondayFirst(const QString& connectionName_,
                                  size_t numBins);

private:
    QDate nextExpectedDate(const QDate& referenceDate) const override;

    QDate normalizeDate(const QDate& date) const override;

    bool compareDate(const QDate& expected,
                     const QDate& probeDate) const override;
};

class QtSprintDistReaderSundayFirst : public DistributionReaderBase {
public:
    QtSprintDistReaderSundayFirst(const QString& connectionName_,
                                  size_t numBins);

private:
    QDate nextExpectedDate(const QDate& referenceDate) const override;

    QDate normalizeDate(const QDate& date) const override;

    bool compareDate(const QDate& expected,
                     const QDate& probeDate) const override;
};

class QtSprintMonthlyDistributionReader : public DistributionReaderBase {
public:
    QtSprintMonthlyDistributionReader(const QString& connectionName_,
                                      size_t numBins);

private:
    QDate nextExpectedDate(const QDate& referenceDate) const override;

    bool compareDate(const QDate& expected,
                     const QDate& probeDate) const override;
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QTSPRINTDISTRIBUTIONREADER_H_TS4GUJR3 */
