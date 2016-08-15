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
#include "qt_storage_impl/QtPomoYearRangeReader.h"
#include "qt_storage_impl/PomodoroDatabase.h"


QtPomoYearRangeReader::QtPomoYearRangeReader(DBService& dbService)
    : dbService{dbService}
{
    connect(&dbService,
            &DBService::results,
            this,
            &QtPomoYearRangeReader::onResultsReceived);
}

void QtPomoYearRangeReader::requestYearRange(Handler handler)
{
    this->handler = handler;
    mQueryId = dbService.executeQuery(QString{
        "SELECT DISTINCT STRFTIME('%Y', %1) "
        "FROM %2 ORDER BY %1;"}.arg(PomodoroTable::Columns::startTime)
                                          .arg(PomodoroTable::name));
}

void QtPomoYearRangeReader::onResultsReceived(
    long long queryId, const std::vector<QSqlRecord>& records)
{
    if (mQueryId != queryId) {
        return;
    }
    std::vector<std::string> range;
    for (const auto& record : records) {
        range.push_back(record.value(0).toString().toStdString());
    }
    handler(range);
}
