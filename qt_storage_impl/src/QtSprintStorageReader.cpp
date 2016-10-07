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
#include "qt_storage_impl/QtSprintStorageReader.h"
#include "qt_storage_impl/Database.h"
#include "utils/DateTimeConverter.h"


QtSprintStorageReader::QtSprintStorageReader(DBService& dbService)
    : dbService{dbService}
{
    mQueryId = dbService.prepare(QString{"SELECT %1, %2, %3, %4, %5, %6, %7 "
                                         "FROM %8 "
                                         "WHERE DATE(%5) >= (:startTime) "
                                         "AND DATE(%5) <= (:finishTime) "
                                         "ORDER BY %5"}
                                     .arg(SprintTable::Columns::id)
                                     .arg(SprintTable::Columns::taskUuid)
                                     .arg(TaskTable::Columns::name)
                                     .arg(SprintView::Aliases::tags)
                                     .arg(SprintTable::Columns::startTime)
                                     .arg(SprintTable::Columns::finishTime)
                                     .arg(TaskTable::Columns::uuid)
                                     .arg(SprintView::name));
    connect(&dbService,
            &DBService::results,
            this,
            &QtSprintStorageReader::onResultsReceived);
}

void QtSprintStorageReader::requestItems(const TimeSpan& timeSpan,
                                       Handler handler)
{
    handler_queue.push_back(handler);
    DateTime start = timeSpan.startTime;
    DateTime finish = timeSpan.finishTime;

    dbService.bind(mQueryId,
                   ":startTime",
                   QVariant(QString::fromStdString(start.toString("yyyy-MM-dd"))));
    dbService.bind(mQueryId,
                   ":finishTime",
                   QVariant(QString::fromStdString(finish.toString("yyyy-MM-dd"))));

    dbService.executePrepared(mQueryId);
}

void QtSprintStorageReader::onResultsReceived(
    long long queryId, const std::vector<QSqlRecord>& records)
{
    if (mQueryId != queryId) {
        return;
    }
    Items sprints;
    std::transform(
        records.cbegin(),
        records.cend(),
        std::back_inserter(sprints),
        [&](const auto& elem) { return this->sprintFromQSqlRecord(elem); });
    handler_queue.front()(sprints);
    handler_queue.pop_front();
}

Sprint QtSprintStorageReader::sprintFromQSqlRecord(const QSqlRecord &record)
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
    return Sprint{name.toStdString(), timeSpan, tags, uuid, taskUuid};
}

QVariant QtSprintStorageReader::columnData(const QSqlRecord& record,
                                         Columns column)
{
    return record.value(static_cast<int>(column));
}
