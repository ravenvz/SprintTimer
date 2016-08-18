/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#include "qt_storage_impl/QtSprintDistributionReader.h"
#include "qt_storage_impl/PomodoroDatabase.h"
#include "utils/DateTimeConverter.h"


DistributionReaderBase::DistributionReaderBase(DBService& dbService)
    : dbService{dbService}
{
    connect(&dbService,
            &DBService::results,
            this,
            &DistributionReaderBase::onResultsReceived);
}

void DistributionReaderBase::requestDailyDistribution(const TimeSpan& timeSpan,
                                                      Handler handler)
{
    handler_queue.push_back(handler);
    QDate from = DateTimeConverter::qDate(timeSpan.startTime);
    QDate now = DateTimeConverter::qDate(timeSpan.finishTime);

    dbService.bind(mQueryId, ":start_date", from);
    dbService.bind(mQueryId, ":start_date", from);
    dbService.bind(mQueryId, ":end_date", now);
    dbService.executePrepared(mQueryId);
}

void DistributionReaderBase::onResultsReceived(
    long long queryId, const std::vector<QSqlRecord>& records)
{
    if (mQueryId != queryId) {
        return;
    }
    std::vector<int> pomodoroCount;
    std::transform(records.cbegin(),
                   records.cend(),
                   std::back_inserter(pomodoroCount),
                   [](const auto& elem) { return elem.value(0).toInt(); });
    handler_queue.front()(Distribution<int>{std::move(pomodoroCount)});
    handler_queue.pop_front();
}

QtSprintDailyDistributionReader::QtSprintDailyDistributionReader(
    DBService& dbService)
    : DistributionReaderBase{dbService}
{
    mQueryId = dbService.prepare(
        QString{"SELECT COUNT(%1) "
                "FROM %2 LEFT JOIN %3 "
                "ON DATE(%4) = %5 "
                "WHERE %5 > DATE(:start_date) AND %5 <= DATE(:end_date)"
                "GROUP BY DATE(%5) "
                "ORDER BY %5"}
            .arg(PomodoroTable::name + "." + PomodoroTable::Columns::id)
            .arg(CalendarTable::name)
            .arg(PomodoroTable::name)
            .arg(PomodoroTable::Columns::startTime)
            .arg(CalendarTable::Columns::dt));
}

QtSprintWeeklyDistributionReader::QtSprintWeeklyDistributionReader(
    DBService& dbService)
    : DistributionReaderBase{dbService}
{
    mQueryId = dbService.prepare(QString{
        "SELECT COUNT(%1) "
        "FROM %2 LEFT JOIN %3 "
        "ON DATE(%1) = %4 "
        "WHERE %4 > (:start_date) AND %4 <= (:end_date) "
        "GROUP BY (STRFTIME('%j', DATE(%4, '-3 days', 'weekday "
        "4')) - 1) / 7 + 1 "
        "ORDER BY %4"}.arg(PomodoroTable::Columns::startTime)
                                     .arg(CalendarTable::name)
                                     .arg(PomodoroTable::name)
                                     .arg(CalendarTable::Columns::dt));
}

QtSprintMonthlyDistributionReader::QtSprintMonthlyDistributionReader(
    DBService& dbService)
    : DistributionReaderBase{dbService}
{
    mQueryId = dbService.prepare(QString{
        "SELECT COUNT(%1) "
        "FROM %2 LEFT JOIN %3 "
        "ON DATE(%1) = %4 "
        "WHERE %4 > (:start_date) AND %4 <= (:end_date) "
        "GROUP BY STRFTIME('%m', %4) "
        "ORDER BY %4"}.arg(PomodoroTable::Columns::startTime)
                                     .arg(CalendarTable::name)
                                     .arg(PomodoroTable::name)
                                     .arg(CalendarTable::Columns::dt));
}
