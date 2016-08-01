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
#include "qt_storage_impl/QtPomoStorageReader.h"
#include "utils/DateTimeConverter.h"


QtPomoStorageReader::QtPomoStorageReader(DBService& dbService)
    : dbService{dbService}
{
    mQueryId = dbService.prepare(
        "select id, todo_uuid, name, tags, start_time, finish_time, uuid "
        "from pomodoro_view "
        "where date(start_time) >= (:startTime) "
        "and date(start_time) <= (:finishTime) "
        "order by start_time");
    connect(&dbService,
            &DBService::results,
            this,
            &QtPomoStorageReader::onResultsReceived);
}

void QtPomoStorageReader::requestItems(const TimeSpan& timeSpan,
                                       Handler handler)
{
    handler_queue.push_back(handler);
    DateTime start = timeSpan.startTime;
    DateTime finish = timeSpan.finishTime;

    dbService.bind(mQueryId,
                   ":startTime",
                   QVariant(QString::fromStdString(start.yyyymmddString())));
    dbService.bind(mQueryId,
                   ":finishTime",
                   QVariant(QString::fromStdString(finish.yyyymmddString())));

    dbService.executePrepared(mQueryId);
}

void QtPomoStorageReader::onResultsReceived(
    long long queryId, const std::vector<QSqlRecord>& records)
{
    if (mQueryId != queryId) {
        return;
    }
    Items pomodoros;
    std::transform(
        records.cbegin(),
        records.cend(),
        std::back_inserter(pomodoros),
        [&](const auto& elem) { return this->pomodoroFromQSqlRecord(elem); });
    handler_queue.front()(pomodoros);
    handler_queue.pop_front();
}

Pomodoro QtPomoStorageReader::pomodoroFromQSqlRecord(const QSqlRecord& record)
{
    QString name{columnData(record, Columns::Name).toString()};
    QDateTime start = columnData(record, Columns::StartTime).toDateTime();
    QDateTime finish = columnData(record, Columns::FinishTime).toDateTime();
    TimeSpan timeSpan{DateTimeConverter::dateTime(start),
                      DateTimeConverter::dateTime(finish)};
    std::string uuid
        = columnData(record, Columns::Uuid).toString().toStdString();
    QStringList tagNames{columnData(record, Columns::Tags)
                             .toString()
                             .split(",", QString::SkipEmptyParts)};
    std::string taskUuid
        = columnData(record, Columns::TodoUuid).toString().toStdString();
    std::list<Tag> tags;
    std::transform(tagNames.cbegin(),
                   tagNames.cend(),
                   std::back_inserter(tags),
                   [](const auto& name) { return Tag{name.toStdString()}; });
    return Pomodoro{name.toStdString(), timeSpan, tags, uuid, taskUuid};
}

QVariant QtPomoStorageReader::columnData(const QSqlRecord& record,
                                         Columns column)
{
    return record.value(static_cast<int>(column));
}