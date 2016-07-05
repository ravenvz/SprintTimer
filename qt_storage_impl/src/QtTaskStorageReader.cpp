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
#include "qt_storage_impl/QtTaskStorageReader.h"
#include "utils/DateTimeConverter.h"


QtTaskStorageReader::QtTaskStorageReader(DBService& dbService)
    : dbService{dbService}
{
    mFinishedQueryId = dbService.prepare(
        "SELECT "
        "id, "
        "name, "
        "estimated_pomodoros, "
        "spent_pomodoros, "
        "priority, "
        "completed ,"
        "tags, "
        "last_modified, "
        "uuid "
        "FROM task_view "
        "WHERE completed = 1 AND date(last_modified) >= (:start_date) "
        "AND date(last_modified) <= (:end_date) "
        "ORDER BY last_modified;");
    connect(&dbService,
            &DBService::results,
            this,
            &QtTaskStorageReader::onResultsReceived);
}

void QtTaskStorageReader::requestUnfinishedTasks(Handler handler)
{
    handler_queue.push_back(handler);
    mUnfinishedQueryId = dbService.executeQuery(
        "SELECT "
        "id, "
        "name, "
        "estimated_pomodoros, "
        "spent_pomodoros, "
        "priority, "
        "completed, "
        "tags, "
        "last_modified, "
        "uuid "
        "FROM task_view "
        "WHERE completed = 0 OR last_modified > datetime('now', '-1 day') "
        "ORDER BY priority;");
}

void QtTaskStorageReader::requestFinishedTasks(const TimeSpan& timeSpan,
                                               Handler handler)
{
    handler_queue.push_back(handler);

    DateTime start = timeSpan.startTime;
    DateTime finish = timeSpan.finishTime;

    dbService.bind(mFinishedQueryId,
                   ":start_date",
                   QVariant(QString::fromStdString(start.yyyymmddString())));
    dbService.bind(mFinishedQueryId,
                   ":end_date",
                   QVariant(QString::fromStdString(finish.yyyymmddString())));
    dbService.executePrepared(mFinishedQueryId);
}

void QtTaskStorageReader::requestAllTags(TagHandler handler)
{
    tag_handler_queue.push_back(handler);
    mTagQueryId = dbService.executeQuery("select id, name from tag "
                                         "order by name;");
}

void QtTaskStorageReader::onResultsReceived(
    long long queryId, const std::vector<QSqlRecord>& records)
{
    if (queryId == mUnfinishedQueryId || queryId == mFinishedQueryId) {
        Items tasks;
        std::transform(
            records.cbegin(),
            records.cend(),
            std::back_inserter(tasks),
            [&](const auto& elem) { return this->taskFromQSqlRecord(elem); });
        handler_queue.front()(tasks);
        handler_queue.pop_front();
    }
    if (queryId == mTagQueryId) {
        std::vector<std::string> tags;
        std::transform(
            records.cbegin(),
            records.cend(),
            std::back_inserter(tags),
            [&](const auto& elem) { return this->tagFromSqlRecord(elem); });
        tag_handler_queue.front()(tags);
        tag_handler_queue.pop_front();
    }
}

Task QtTaskStorageReader::taskFromQSqlRecord(const QSqlRecord& record) const
{
    std::string name{columnData(record, Column::Name).toString().toStdString()};
    std::string uuid{columnData(record, Column::Uuid).toString().toStdString()};
    int estimatedPomodoros{
        columnData(record, Column::EstimatedPomodoros).toInt()};
    int spentPomodoros{columnData(record, Column::SpentPomodoros).toInt()};
    QStringList tagNames{columnData(record, Column::Tags)
                             .toString()
                             .split(",", QString::SkipEmptyParts)};
    std::list<Tag> tags;
    std::transform(tagNames.cbegin(),
                   tagNames.cend(),
                   std::back_inserter(tags),
                   [](const auto& tag) { return Tag{tag.toStdString()}; });
    bool finished{columnData(record, Column::Completed).toBool()};
    QDateTime qLastModified{
        columnData(record, Column::LastModified).toDateTime()};
    DateTime lastModified = DateTimeConverter::dateTime(qLastModified);
    return Task{name,
                    estimatedPomodoros,
                    spentPomodoros,
                    uuid,
                    tags,
                    finished,
                    lastModified};
}

std::string
QtTaskStorageReader::tagFromSqlRecord(const QSqlRecord& record) const
{
    return record.value(static_cast<int>(TagColumn::Name))
        .toString()
        .toStdString();
}

QVariant QtTaskStorageReader::columnData(const QSqlRecord& record,
                                         Column column) const
{
    return record.value(static_cast<int>(column));
}
