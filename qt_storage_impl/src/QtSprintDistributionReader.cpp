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
#include "qt_storage_impl/QtSprintDistributionReader.h"
#include "qt_storage_impl/Database.h"
#include "utils/DateTimeConverter.h"


namespace {

auto equalByDay = [](const QDate& referenceDate, const QDate& date) {
    return referenceDate == date;
};

auto incrementByDay = [](const QDate& date) { return date.addDays(1); };

auto equalByWeek = [](const QDate& referenceDate, const QDate& date) {
    return referenceDate.weekNumber() == date.weekNumber();
};

auto incrementByWeek = [](const QDate& date) { return date.addDays(7); };

auto equalByMonth = [](const QDate& referenceDate, const QDate& date) {
    return referenceDate.month() == date.month();
};

auto incrementByMonth = [](const QDate& date) { return date.addMonths(1); };
}

DistributionReaderBase::DistributionReaderBase(
    DBService& dbService,
    size_t distributionSize,
    std::function<bool(const QDate&, const QDate&)> compareFunc,
    std::function<QDate(const QDate&)> incrementFunc)
    : dbService{dbService}
    , distributionSize{distributionSize}
    , equalityFunc{compareFunc}
    , incrementFunc{incrementFunc}

{
    connect(&dbService,
            &DBService::results,
            this,
            &DistributionReaderBase::onResultsReceived);
}

void DistributionReaderBase::requestDistribution(const TimeSpan& timeSpan,
                                                 Handler handler)
{
    handler_queue.push_back(handler);
    startDate = DateTimeConverter::qDate(timeSpan.startTime);
    QDate endDate = DateTimeConverter::qDate(timeSpan.finishTime);

    dbService.bind(mQueryId, ":start_date", startDate);
    dbService.bind(mQueryId, ":end_date", endDate);
    dbService.executePrepared(mQueryId);
}

void DistributionReaderBase::executeCallback(std::vector<int>&& sprintCount)
{
    handler_queue.front()(Distribution<int>{std::move(sprintCount)});
    handler_queue.pop_front();
}

bool DistributionReaderBase::invalidQueryId(long long int queryId) const
{
    return mQueryId != queryId;
}

void DistributionReaderBase::onResultsReceived(
    long long queryId, const std::vector<QSqlRecord>& records)
{
    if (invalidQueryId(queryId))
        return;
    auto distribution = fillDateGaps(records, equalityFunc, incrementFunc);
    executeCallback(std::move(distribution));
}

QtSprintDailyDistributionReader::QtSprintDailyDistributionReader(
    DBService& dbService, size_t numBins)
    : DistributionReaderBase{dbService, numBins, equalByDay, incrementByDay}
{
    mQueryId = dbService.prepare(QString{
        "SELECT COUNT(*), DATE(%1) "
        "FROM %2 WHERE DATE(%1) >= DATE(:start_date) "
        "AND DATE(%1) <= DATE(:end_date) "
        "GROUP BY DATE(%1) "
        "ORDER BY DATE(%1)"}.arg(SprintTable::Columns::startTime)
                            .arg(SprintTable::name));
}

QtSprintWeeklyDistributionReader::QtSprintWeeklyDistributionReader(
    DBService& dbService, size_t numBins)
    : DistributionReaderBase{dbService, numBins, equalByWeek, incrementByWeek}
{
    mQueryId = dbService.prepare(QString{
        "SELECT COUNT(*), %1 "
        "FROM %2 WHERE DATE(%1) >= (:start_date) AND DATE(%1) <= (:end_date) "
        "GROUP BY (STRFTIME('%j', DATE(%1, '-3 days', 'weekday 4')) - 1) / 7 + "
        "1 "
        "ORDER BY DATE(%1)"}.arg(SprintTable::Columns::startTime)
                                     .arg(SprintTable::name));
}

QtSprintMonthlyDistributionReader::QtSprintMonthlyDistributionReader(
    DBService& dbService, size_t numBins)
    : DistributionReaderBase{dbService, numBins, equalByMonth, incrementByMonth}
{
    mQueryId = dbService.prepare(QString{
        "SELECT COUNT(*), %1 "
        "FROM %2 WHERE DATE(%1) >= (:start_date) AND DATE(%1) <= (:end_date) "
        "GROUP BY STRFTIME('%m', DATE(%1)) "
        "ORDER BY DATE(%1)"}.arg(SprintTable::Columns::startTime)
                                     .arg(SprintTable::name));
}
