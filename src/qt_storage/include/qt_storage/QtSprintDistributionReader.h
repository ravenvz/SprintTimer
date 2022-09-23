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

#include "api/SprintDistributionReader.h"
#include <QDate>

#include <QSqlQuery>

namespace sprint_timer::storage::qt_storage {

class DistributionReaderBase : public SprintDistributionReader {
public:
    DistributionReaderBase(QString connectionName, size_t distributionSize);

    DistributionReaderBase(DistributionReaderBase&&) = delete;
    DistributionReaderBase(const DistributionReaderBase&) = delete;

    auto operator=(DistributionReaderBase&&)
        -> DistributionReaderBase& = delete;
    auto operator=(const DistributionReaderBase&)
        -> DistributionReaderBase& = delete;

    auto sprintDistribution(const dw::DateRange& dateRange)
        -> std::vector<int> override;

protected:
    QString connectionName;
    QSqlQuery rangeQuery;

    [[nodiscard]] virtual auto
    nextExpectedDate(const QDate& referenceDate) const -> QDate = 0;

    [[nodiscard]] virtual auto normalizeDate(const QDate& date) const -> QDate;

    [[nodiscard]] virtual auto compareDate(const QDate& expected,
                                           const QDate& probeDate) const
        -> bool;

    [[nodiscard]] auto zeroFilledDistribution(
        const QDate& startDate,
        const std::vector<std::pair<QDate, int>>& query) const
        -> std::vector<int>;

private:
    size_t distributionSize;
};

class QtSprintDailyDistributionReader : public DistributionReaderBase {
public:
    QtSprintDailyDistributionReader(QString connectionName, size_t numBins);

private:
    [[nodiscard]] auto nextExpectedDate(const QDate& referenceDate) const
        -> QDate override;
};

class QtSprintDistReaderMondayFirst : public DistributionReaderBase {
public:
    QtSprintDistReaderMondayFirst(QString connectionName, size_t numBins);

private:
    [[nodiscard]] auto nextExpectedDate(const QDate& referenceDate) const
        -> QDate override;

    [[nodiscard]] auto normalizeDate(const QDate& date) const -> QDate override;

    [[nodiscard]] auto compareDate(const QDate& expected,
                                   const QDate& probeDate) const
        -> bool override;
};

class QtSprintDistReaderSundayFirst : public DistributionReaderBase {
public:
    QtSprintDistReaderSundayFirst(QString connectionName, size_t numBins);

private:
    [[nodiscard]] auto nextExpectedDate(const QDate& referenceDate) const
        -> QDate override;

    [[nodiscard]] auto normalizeDate(const QDate& date) const -> QDate override;

    [[nodiscard]] auto compareDate(const QDate& expected,
                                   const QDate& probeDate) const
        -> bool override;
};

class QtSprintMonthlyDistributionReader : public DistributionReaderBase {
public:
    QtSprintMonthlyDistributionReader(QString connectionName, size_t numBins);

private:
    [[nodiscard]] auto nextExpectedDate(const QDate& referenceDate) const
        -> QDate override;

    [[nodiscard]] auto compareDate(const QDate& expected,
                                   const QDate& probeDate) const
        -> bool override;
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QTSPRINTDISTRIBUTIONREADER_H_TS4GUJR3 */
