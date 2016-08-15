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
#include "qt_storage_impl/QtPomoDistributionReader.h"
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

QtPomoDailyDistributionReader::QtPomoDailyDistributionReader(
    DBService& dbService)
    : DistributionReaderBase{dbService}
{
    mQueryId = dbService.prepare(
        "select count(pomodoro.id) "
        "from calendar left join pomodoro "
        "on date(start_time) = dt "
        "where dt > date(:start_date) and dt <= date(:end_date)"
        "group by date(dt) "
        "order by dt");
}

QtPomoWeeklyDistributionReader::QtPomoWeeklyDistributionReader(
    DBService& dbService)
    : DistributionReaderBase{dbService}
{
    mQueryId = dbService.prepare(
        "select count(start_time) "
        "from calendar left join pomodoro "
        "on date(start_time) = dt "
        "where dt > (:start_date) and dt <= (:end_date) "
        "group by (strftime('%j', date(dt, '-3 days', 'weekday "
        "4')) - 1) / 7 + 1 "
        "order by dt");
}

QtPomoMonthlyDistributionReader::QtPomoMonthlyDistributionReader(
    DBService& dbService)
    : DistributionReaderBase{dbService}
{
    mQueryId
        = dbService.prepare("select count(start_time) "
                            "from calendar left join pomodoro "
                            "on date(start_time) = dt "
                            "where dt > (:start_date) and dt <= (:end_date) "
                            "group by strftime('%m', dt) "
                            "order by dt");
}
