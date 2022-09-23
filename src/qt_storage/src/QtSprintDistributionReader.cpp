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
#include "qt_storage/QtSprintDistributionReader.h"
#include "qt_storage/DatabaseDescription.h"
#include "qt_storage/utils/DateTimeConverter.h"
#include "qt_storage/utils/QueryUtils.h"

namespace {

constexpr int daysInWeek{7};

std::vector<std::pair<QDate, int>> unfilledDistribution(QSqlQuery& query);

} // namespace

namespace sprint_timer::storage::qt_storage {

DistributionReaderBase::DistributionReaderBase(QString connectionName_,
                                               size_t distributionSize_)
    : connectionName{std::move(connectionName_)}
    , distributionSize{distributionSize_}
{
}

auto DistributionReaderBase::sprintDistribution(const dw::DateRange& dateRange)
    -> std::vector<int>
{
    const storage::utils::DateConverter dateConverter;
    const QDate startDate = dateConverter(dateRange.start());
    const QDate endDate = dateConverter(dateRange.finish());

    rangeQuery.bindValue(":start_date", startDate);
    rangeQuery.bindValue(":end_date", endDate);

    qt_storage::tryExecute(rangeQuery);

    const auto unfilled = unfilledDistribution(rangeQuery);
    rangeQuery.finish();

    return zeroFilledDistribution(startDate, unfilled);
}

auto DistributionReaderBase::zeroFilledDistribution(
    const QDate& startDate,
    const std::vector<std::pair<QDate, int>>& unfilled) const
    -> std::vector<int>
{
    std::vector<int> sprintCount(distributionSize, 0);

    QDate expected = normalizeDate(startDate);
    auto recordIter = cbegin(unfilled);
    for (auto& elem : sprintCount) {
        if (recordIter == cend(unfilled)) {
            break;
        }
        const auto& [date, value] = *recordIter;
        if (compareDate(expected, date)) {
            elem = value;
            ++recordIter;
        }
        expected = nextExpectedDate(expected);
    }

    return sprintCount;
}

auto DistributionReaderBase::normalizeDate(const QDate& date) const -> QDate
{
    return date;
}

auto DistributionReaderBase::compareDate(const QDate& expected,
                                         const QDate& probeDate) const -> bool
{
    return expected == probeDate;
}

QtSprintDailyDistributionReader::QtSprintDailyDistributionReader(
    QString connectionName_, size_t numBins_)
    : DistributionReaderBase{std::move(connectionName_), numBins_}
{
    rangeQuery = tryPrepare(connectionName,
                            QString{"SELECT COUNT(*), DATE(%1) "
                                    "FROM %2 WHERE "
                                    "DATE(%1) >= DATE(:start_date) "
                                    "AND DATE(%1) <= DATE(:end_date) "
                                    "GROUP BY DATE(%1) "
                                    "ORDER BY DATE(%1)"}
                                .arg(SprintTable::Columns::startTime)
                                .arg(CleanSprintView::name));
}

auto QtSprintDailyDistributionReader::nextExpectedDate(
    const QDate& referenceDate) const -> QDate
{
    return referenceDate.addDays(1);
}

QtSprintDistReaderMondayFirst::QtSprintDistReaderMondayFirst(
    QString connectionName_, size_t numBins_)
    : DistributionReaderBase{std::move(connectionName_), numBins_}
{
    rangeQuery = tryPrepare(connectionName,
                            QString{"SELECT COUNT(*), start_time "
                                    "FROM %2 WHERE "
                                    "DATE(%1) >= (:start_date) "
                                    "AND DATE(%1) <= (:end_date) "
                                    "GROUP BY (STRFTIME('%j', DATE(%1, '-3 "
                                    "days', 'weekday 4')) - 1) / 7 + "
                                    "1 "
                                    "ORDER BY DATE(%1)"}
                                .arg(SprintTable::Columns::startTime)
                                .arg(CleanSprintView::name));
}

auto QtSprintDistReaderMondayFirst::nextExpectedDate(
    const QDate& referenceDate) const -> QDate
{
    return referenceDate.addDays(daysInWeek);
}

auto QtSprintDistReaderMondayFirst::normalizeDate(const QDate& date) const
    -> QDate
{
    return date;
}

auto QtSprintDistReaderMondayFirst::compareDate(const QDate& expected,
                                                const QDate& probeDate) const
    -> bool
{
    return expected.weekNumber() == probeDate.weekNumber();
}

QtSprintDistReaderSundayFirst::QtSprintDistReaderSundayFirst(
    QString connectionName_, size_t numBins_)
    : DistributionReaderBase{std::move(connectionName_), numBins_}
{
    rangeQuery =
        tryPrepare(connectionName,
                   QString{"SELECT COUNT(*), DATE(%1, 'weekday 6') AS saturday "
                           "FROM %2 WHERE "
                           "DATE(%1) >= (:start_date) "
                           "AND date(%1) <= (:end_date) "
                           "GROUP BY saturday "
                           "ORDER BY DATE(%1)"}
                       .arg(SprintTable::Columns::startTime)
                       .arg(CleanSprintView::name));
}

auto QtSprintDistReaderSundayFirst::nextExpectedDate(
    const QDate& referenceDate) const -> QDate
{
    return referenceDate.addDays(daysInWeek);
}

auto QtSprintDistReaderSundayFirst::normalizeDate(const QDate& date) const
    -> QDate
{
    if (date.dayOfWeek() == Qt::DayOfWeek::Sunday) {
        return date.addDays(6);
    }
    return date.addDays(Qt::DayOfWeek::Saturday - date.dayOfWeek());
}

auto QtSprintDistReaderSundayFirst::compareDate(const QDate& expected,
                                                const QDate& probeDate) const
    -> bool
{
    return expected == probeDate;
}

QtSprintMonthlyDistributionReader::QtSprintMonthlyDistributionReader(
    QString connectionName_, size_t numBins_)
    : DistributionReaderBase{std::move(connectionName_), numBins_}
{
    rangeQuery = tryPrepare(connectionName,
                            QString{"SELECT COUNT(*), start_time "
                                    "FROM %2 WHERE "
                                    "DATE(%1) >= (:start_date) "
                                    "AND DATE(%1) <= (:end_date) "
                                    "GROUP BY STRFTIME('%m', DATE(%1)) "
                                    "ORDER BY DATE(%1)"}
                                .arg(SprintTable::Columns::startTime)
                                .arg(CleanSprintView::name));
}

auto QtSprintMonthlyDistributionReader::nextExpectedDate(
    const QDate& referenceDate) const -> QDate
{
    return referenceDate.addMonths(1);
}

auto QtSprintMonthlyDistributionReader::compareDate(
    const QDate& expected, const QDate& probeDate) const -> bool
{
    return expected.month() == probeDate.month();
}

} // namespace sprint_timer::storage::qt_storage

namespace {

auto unfilledDistribution(QSqlQuery& query)
    -> std::vector<std::pair<QDate, int>>
{
    using namespace sprint_timer::storage::qt_storage;
    const auto records = copyAllRecords(query);
    std::vector<std::pair<QDate, int>> distribution;
    distribution.reserve(records.size());
    std::ranges::transform(
        records,
        std::back_inserter(distribution),
        [](const auto& record) -> std::pair<QDate, int> {
            return {record.value(1).toDate(), record.value(0).toInt()};
        });
    return distribution;
}

} // namespace
