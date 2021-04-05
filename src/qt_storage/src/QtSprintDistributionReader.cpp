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
#include "qt_storage/QtSprintDistributionReader.h"
#include "qt_storage/DatabaseDescription.h"
#include "qt_storage/utils/DateTimeConverter.h"
#include "qt_storage/utils/QueryUtils.h"

namespace {

constexpr int daysInWeek{7};

std::vector<std::pair<QDate, int>> unfilledDistribution(QSqlQuery& query);

} // namespace

namespace sprint_timer::storage::qt_storage {

DistributionReaderBase::DistributionReaderBase(size_t distributionSize_)
    : distributionSize{std::move(distributionSize_)}
{
}

std::vector<int>
DistributionReaderBase::sprintDistribution(const dw::DateRange& dateRange)
{
    using namespace storage::utils;
    const QDate startDate = DateTimeConverter::qDate(dateRange.start());
    const QDate endDate = DateTimeConverter::qDate(dateRange.finish());

    rangeQuery.bindValue(":start_date", startDate);
    rangeQuery.bindValue(":end_date", endDate);

    qt_storage::tryExecute(rangeQuery);

    const auto unfilled = unfilledDistribution(rangeQuery);
    rangeQuery.finish();

    return zeroFilledDistribution(startDate, unfilled);
}

std::vector<int> DistributionReaderBase::zeroFilledDistribution(
    const QDate& startDate,
    const std::vector<std::pair<QDate, int>>& unfilled) const
{
    std::vector<int> sprintCount(distributionSize, 0);

    QDate expected = normalizeDate(startDate);
    auto recordIter = cbegin(unfilled);
    for (auto& elem : sprintCount) {
        if (recordIter == cend(unfilled))
            break;
        const auto& [date, value] = *recordIter;
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
    const QString& connectionName_, size_t numBins_)
    : DistributionReaderBase{numBins_}
{
    using namespace qt_storage;
    rangeQuery =
        tryPrepare(connectionName_,
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
    const QString& connectionName_, size_t numBins_)
    : DistributionReaderBase{numBins_}
{
    using namespace qt_storage;
    rangeQuery = tryPrepare(connectionName_,
                            QString{"SELECT COUNT(*), start_time "
                                    "FROM %2 WHERE DATE(%1) >= (:start_date) "
                                    "AND DATE(%1) <= (:end_date) "
                                    "GROUP BY (STRFTIME('%j', DATE(%1, '-3 "
                                    "days', 'weekday 4')) - 1) / 7 + "
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
    const QString& connectionName_, size_t numBins_)
    : DistributionReaderBase{numBins_}
{
    using namespace qt_storage;
    rangeQuery =
        tryPrepare(connectionName_,
                   QString{"SELECT COUNT(*), DATE(%1, 'weekday 6') AS saturday "
                           "FROM %2 WHERE DATE(%1) >= (:start_date) AND "
                           "date(%1) <= (:end_date) "
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
    const QString& connectionName_, size_t numBins_)
    : DistributionReaderBase{numBins_}
{
    using namespace qt_storage;
    rangeQuery = tryPrepare(connectionName_,
                            QString{"SELECT COUNT(*), start_time "
                                    "FROM %2 WHERE DATE(%1) >= (:start_date) "
                                    "AND DATE(%1) <= (:end_date) "
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

} // namespace sprint_timer::storage::qt_storage

namespace {

std::vector<std::pair<QDate, int>> unfilledDistribution(QSqlQuery& query)
{
    using namespace sprint_timer::storage::qt_storage;
    const auto records = copyAllRecords(query);
    std::vector<std::pair<QDate, int>> distribution;
    distribution.reserve(records.size());
    std::transform(
        records.cbegin(),
        records.cend(),
        std::back_inserter(distribution),
        [](const auto& record) -> std::pair<QDate, int> {
            return {record.value(1).toDate(), record.value(0).toInt()};
        });
    return distribution;
}

} // namespace
