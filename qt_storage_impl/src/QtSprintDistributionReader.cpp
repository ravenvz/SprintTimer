/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#include "qt_storage_impl/QtSprintDistributionReader.h"
#include "qt_storage_impl/DatabaseDescription.h"
#include "qt_storage_impl/utils/DateTimeConverter.h"

namespace {

constexpr int daysInWeek{7};

// bool equalByDay(const QDate& referenceDate, const QDate& date)
// {
//     return referenceDate == date;
// }
//
// QDate incrementByDay(const QDate& date) { return date.addDays(1); }
//
// bool equalByWeek(const QDate& referenceDate, const QDate& date)
// {
//     return referenceDate.weekNumber() == date.weekNumber();
// }
//
// QDate incrementByWeek(const QDate& date) { return date.addDays(7); }
//
// bool equalByMonth(const QDate& referenceDate, const QDate& date)
// {
//     return referenceDate.month() == date.month();
// }
//
// QDate incrementByMonth(const QDate& date) { return date.addMonths(1); }

} // namespace

namespace sprint_timer::storage::qt_storage_impl {

DistributionReaderBase::DistributionReaderBase(DBService& dbService,
                                               size_t distributionSize)
    : dbService{dbService}
    , distributionSize{distributionSize}

{
    connect(&dbService,
            &DBService::results,
            this,
            &DistributionReaderBase::onResultsReceived);
}

void DistributionReaderBase::requestDistribution(const dw::DateRange& dateRange,
                                                 Handler handler)
{
    using namespace storage::utils;
    handler_queue.push_back(handler);
    startDate = DateTimeConverter::qDate(dateRange.start());
    const QDate endDate = DateTimeConverter::qDate(dateRange.finish());

    dbService.bind(mQueryId, ":start_date", startDate);
    dbService.bind(mQueryId, ":end_date", endDate);
    dbService.executePrepared(mQueryId);
}

void DistributionReaderBase::executeCallback(std::vector<int>&& sprintCount)
{
    handler_queue.front()(Distribution<int>{std::move(sprintCount)});
    handler_queue.pop_front();
}

bool DistributionReaderBase::invalidQueryId(qint64 queryId) const
{
    return mQueryId != queryId;
}

void DistributionReaderBase::onResultsReceived(
    qint64 queryId, const std::vector<QSqlRecord>& records)
{
    if (invalidQueryId(queryId))
        return;
    auto distribution = fillDateGaps(records);
    executeCallback(std::move(distribution));
}

std::vector<int> DistributionReaderBase::fillDateGaps(
    const std::vector<QSqlRecord>& records) const
{
    std::vector<int> sprintCount(distributionSize, 0);

    QDate expected = normalizeDate(startDate);
    auto recordIter = cbegin(records);
    // auto sprintCountIter = begin(sprintCount);
    for (auto& elem : sprintCount) {
        if (recordIter == cend(records))
            break;
        const QDate date = recordIter->value(1).toDate();
        const int value = recordIter->value(0).toInt();
        if (compareDate(expected, date)) {
            elem = value;
            ++recordIter;
        }
        expected = nextExpectedDate(expected);
    }

    return sprintCount;
}

QDate DistributionReaderBase::normalizeDate(const QDate& date) const
{
    return date;
}

bool DistributionReaderBase::compareDate(const QDate& expected,
                                         const QDate& probeDate) const
{
    return expected == probeDate;
}


QtSprintDailyDistributionReader::QtSprintDailyDistributionReader(
    DBService& dbService_, size_t numBins)
    : DistributionReaderBase{dbService_, numBins}
{
    mQueryId = dbService.prepare(
        QString{"SELECT COUNT(*), DATE(%1) "
                "FROM %2 WHERE DATE(%1) >= DATE(:start_date) "
                "AND DATE(%1) <= DATE(:end_date) "
                "GROUP BY DATE(%1) "
                "ORDER BY DATE(%1)"}
            .arg(SprintTable::Columns::startTime)
            .arg(SprintTable::name));
}

QDate QtSprintDailyDistributionReader::nextExpectedDate(
    const QDate& referenceDate) const
{
    return referenceDate.addDays(1);
}


QtSprintDistReaderMondayFirst::QtSprintDistReaderMondayFirst(
    DBService& dbService_, size_t numBins)
    : DistributionReaderBase{dbService_, numBins}
{
    mQueryId = dbService.prepare(QString{
        "SELECT COUNT(*), start_time "
        "FROM %2 WHERE DATE(%1) >= (:start_date) AND DATE(%1) <= (:end_date) "
        "GROUP BY (STRFTIME('%j', DATE(%1, '-3 days', 'weekday 4')) - 1) / 7 + "
        "1 "
        "ORDER BY DATE(%1)"}
                                     .arg(SprintTable::Columns::startTime)
                                     .arg(SprintTable::name));
}

QDate QtSprintDistReaderMondayFirst::nextExpectedDate(
    const QDate& referenceDate) const
{
    return referenceDate.addDays(daysInWeek);
}

QDate QtSprintDistReaderMondayFirst::normalizeDate(const QDate& date) const
{
    return date;
}

bool QtSprintDistReaderMondayFirst::compareDate(const QDate& expected,
                                                const QDate& probeDate) const
{
    return expected.weekNumber() == probeDate.weekNumber();
}


QtSprintDistReaderSundayFirst::QtSprintDistReaderSundayFirst(
    DBService& dbService_, size_t numBins)
    : DistributionReaderBase{dbService_, numBins}
{
    mQueryId = dbService.prepare(QString{
        "SELECT COUNT(*), DATE(%1, 'weekday 6') AS saturday "
        "FROM %2 WHERE DATE(%1) >= (:start_date) AND date(%1) <= (:end_date) "
        "GROUP BY saturday "
        "ORDER BY DATE(%1)"}
                                     .arg(SprintTable::Columns::startTime)
                                     .arg(SprintTable::name));
}

QDate QtSprintDistReaderSundayFirst::nextExpectedDate(
    const QDate& referenceDate) const
{
    return referenceDate.addDays(daysInWeek);
}

QDate QtSprintDistReaderSundayFirst::normalizeDate(const QDate& date) const
{
    if (date.dayOfWeek() == Qt::DayOfWeek::Sunday)
        return date.addDays(6);
    return date.addDays(Qt::DayOfWeek::Saturday - date.dayOfWeek());
}

bool QtSprintDistReaderSundayFirst::compareDate(const QDate& expected,
                                                const QDate& probeDate) const
{
    return expected == probeDate;
}


QtSprintMonthlyDistributionReader::QtSprintMonthlyDistributionReader(
    DBService& dbService_, size_t numBins)
    : DistributionReaderBase{dbService_, numBins}
{
    mQueryId = dbService.prepare(QString{
        "SELECT COUNT(*), start_time "
        "FROM %2 WHERE DATE(%1) >= (:start_date) AND DATE(%1) <= (:end_date) "
        "GROUP BY STRFTIME('%m', DATE(%1)) "
        "ORDER BY DATE(%1)"}
                                     .arg(SprintTable::Columns::startTime)
                                     .arg(SprintTable::name));
}

QDate QtSprintMonthlyDistributionReader::nextExpectedDate(
    const QDate& referenceDate) const
{
    return referenceDate.addMonths(1);
}

bool QtSprintMonthlyDistributionReader::compareDate(
    const QDate& expected, const QDate& probeDate) const
{
    return expected.month() == probeDate.month();
}

} // namespace sprint_timer::storage::qt_storage_impl
