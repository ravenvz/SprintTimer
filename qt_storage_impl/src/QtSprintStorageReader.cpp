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
#include "qt_storage_impl/QtSprintStorageReader.h"
#include "qt_storage_impl/Database.h"
#include "qt_storage_impl/DatabaseDescription.h"
#include "qt_storage_impl/utils/DateTimeConverter.h"

namespace sprint_timer::storage::qt_storage_impl {

using namespace dw;
using namespace storage::utils;
using namespace entities;

QtSprintStorageReader::QtSprintStorageReader(DBService& dbService)
    : dbService{dbService}
{
    sprintsInTimeRangeQueryId
        = dbService.prepare(QString{"SELECT %1, %2, %3, %4, %5, %6, %7 "
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
    sprintsForTaskQueryId
        = dbService.prepare(QString{"SELECT %1, %2, %3, %4, %5, %6, %7 "
                                    "FROM %8 "
                                    "WHERE %2 = (:taskUuid) "
                                    "ORDER by %5"}
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
    DateTime start = timeSpan.start();
    DateTime finish = timeSpan.finish();

    dbService.bind(
        sprintsInTimeRangeQueryId,
        ":startTime",
        QVariant(QString::fromStdString(start.toString("yyyy-MM-dd"))));
    dbService.bind(
        sprintsInTimeRangeQueryId,
        ":finishTime",
        QVariant(QString::fromStdString(finish.toString("yyyy-MM-dd"))));

    dbService.executePrepared(sprintsInTimeRangeQueryId);
}

void QtSprintStorageReader::sprintsForTask(const std::string& taskUuid,
                                           Handler handler)
{
    handler_queue.push_back(handler);
    dbService.bind(sprintsForTaskQueryId,
                   ":taskUuid",
                   QVariant(QString::fromStdString(taskUuid)));
    dbService.executePrepared(sprintsForTaskQueryId);
}

void QtSprintStorageReader::onResultsReceived(
    qint64 queryId, const std::vector<QSqlRecord>& records)
{
    if (!listeningToQueryId(queryId)) {
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

Sprint QtSprintStorageReader::sprintFromQSqlRecord(const QSqlRecord& record)
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
                   [](const auto& nm) { return Tag{nm.toStdString()}; });
    return Sprint{name.toStdString(), timeSpan, tags, uuid, taskUuid};
}

QVariant QtSprintStorageReader::columnData(const QSqlRecord& record,
                                           Columns column)
{
    return record.value(static_cast<int>(column));
}

bool QtSprintStorageReader::listeningToQueryId(qint64 queryId) const
{
    return sprintsInTimeRangeQueryId == queryId
        || sprintsForTaskQueryId == queryId;
}

} // namespace sprint_timer::storage::qt_storage_impl
