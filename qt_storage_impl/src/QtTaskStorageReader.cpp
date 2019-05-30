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
#include "qt_storage_impl/QtTaskStorageReader.h"
#include "qt_storage_impl/DatabaseDescription.h"
#include "qt_storage_impl/utils/DateTimeConverter.h"

namespace sprint_timer::storage::qt_storage_impl {

using namespace entities;

QtTaskStorageReader::QtTaskStorageReader(DBService& dbService)
    : dbService{dbService}
{
    finishedQueryId = dbService.prepare(
        QString{"SELECT %1, %2, %3, %4, %5, %6, %7, %8, %9 "
                "FROM %10 "
                "WHERE %6 = 1 AND DATE(%8) >= (:start_date) "
                "AND DATE(%8) <= (:end_date) "
                "ORDER BY %8;"}
            .arg(TaskTable::Columns::id)
            .arg(TaskTable::Columns::name)
            .arg(TaskTable::Columns::estimatedCost)
            .arg(TaskTable::Columns::actualCost)
            .arg(TaskTable::Columns::priority)
            .arg(TaskTable::Columns::completed)
            .arg(TasksView::Aliases::tags)
            .arg(TaskTable::Columns::lastModified)
            .arg(TaskTable::Columns::uuid)
            .arg(TasksView::name));
    requestTasksQueryId = dbService.prepare(
        QString{"SELECT %1, %2, %3, %4, %5, %6, %7, %8, %9 "
                "FROM %10 "
                "WHERE DATE(%8) >= (:start_date) AND DATE(%8) <= (:end_date) "
                "ORDER BY %8;"}
            .arg(TaskTable::Columns::id)
            .arg(TaskTable::Columns::name)
            .arg(TaskTable::Columns::estimatedCost)
            .arg(TaskTable::Columns::actualCost)
            .arg(TaskTable::Columns::priority)
            .arg(TaskTable::Columns::completed)
            .arg(TasksView::Aliases::tags)
            .arg(TaskTable::Columns::lastModified)
            .arg(TaskTable::Columns::uuid)
            .arg(TasksView::name));
    connect(&dbService,
            &DBService::results,
            this,
            &QtTaskStorageReader::onResultsReceived);
}

void QtTaskStorageReader::requestUnfinishedTasks(Handler handler)
{
    handlerQueue.push(handler);
    unfinishedQueryQueue.push(dbService.execute(
        QString{"SELECT %1, %2, %3, %4, %5, %6, %7, %8, %9 "
                "FROM %10 "
                "WHERE %6 = 0 OR %8 > DATETIME('now', '-1 day') "
                "ORDER BY %5;"}
            .arg(TaskTable::Columns::id)
            .arg(TaskTable::Columns::name)
            .arg(TaskTable::Columns::estimatedCost)
            .arg(TaskTable::Columns::actualCost)
            .arg(TaskTable::Columns::priority)
            .arg(TaskTable::Columns::completed)
            .arg(TasksView::Aliases::tags)
            .arg(TaskTable::Columns::lastModified)
            .arg(TaskTable::Columns::uuid)
            .arg(TasksView::name)));
}

void QtTaskStorageReader::requestFinishedTasks(const dw::DateRange& dateRange,
                                               Handler handler)
{
    handlerQueue.push(handler);

    dbService.bind(finishedQueryId,
                   ":start_date",
                   QVariant(QString::fromStdString(
                       dw::to_string(dateRange.start(), "yyyy-MM-dd"))));
    dbService.bind(finishedQueryId,
                   ":end_date",
                   QVariant(QString::fromStdString(
                       dw::to_string(dateRange.finish(), "yyyy-MM-dd"))));
    dbService.executePrepared(finishedQueryId);
}

void QtTaskStorageReader::requestTasks(const dw::DateRange& dateRange,
                                       Handler handler)
{
    handlerQueue.push(handler);

    dbService.bind(requestTasksQueryId,
                   ":start_date",
                   QVariant(QString::fromStdString(
                       dw::to_string(dateRange.start(), "yyyy-MM-dd"))));
    dbService.bind(requestTasksQueryId,
                   ":end_date",
                   QVariant(QString::fromStdString(
                       dw::to_string(dateRange.finish(), "yyyy-MM-dd"))));
    dbService.executePrepared(requestTasksQueryId);
}

void QtTaskStorageReader::requestAllTags(TagHandler handler)
{
    tagHandlerQueue.push(handler);
    tagQueryId = dbService.execute(QString{"SELECT %1, %2 FROM %3 "
                                           "ORDER BY %2;"}
                                       .arg(TagTable::Columns::id)
                                       .arg(TagTable::Columns::name)
                                       .arg(TagTable::name));
}

void QtTaskStorageReader::onResultsReceived(
    qint64 queryId, const std::vector<QSqlRecord>& records)
{
    if (validTaskQueryId(queryId)) {
        Items tasks;
        std::transform(
            records.cbegin(),
            records.cend(),
            std::back_inserter(tasks),
            [&](const auto& elem) { return this->taskFromQSqlRecord(elem); });
        handlerQueue.front()(tasks);
        handlerQueue.pop();
        if (!unfinishedQueryQueue.empty()
            && queryId == unfinishedQueryQueue.front())
            unfinishedQueryQueue.pop();
    }
    if (validTagQueryId(queryId)) {
        std::vector<std::string> tags;
        std::transform(
            records.cbegin(),
            records.cend(),
            std::back_inserter(tags),
            [&](const auto& elem) { return this->tagFromSqlRecord(elem); });
        tagHandlerQueue.front()(tags);
        tagHandlerQueue.pop();
    }
}

bool QtTaskStorageReader::validTaskQueryId(qint64 queryId) const
{
    return (!unfinishedQueryQueue.empty()
            && queryId == unfinishedQueryQueue.front())
        || queryId == finishedQueryId || queryId == requestTasksQueryId;
}

bool QtTaskStorageReader::validTagQueryId(qint64 queryId) const
{
    return queryId == tagQueryId;
}

Task QtTaskStorageReader::taskFromQSqlRecord(const QSqlRecord& record) const
{
    std::string name{columnData(record, Column::Name).toString().toStdString()};
    std::string uuid{columnData(record, Column::Uuid).toString().toStdString()};
    int estimatedCost{columnData(record, Column::EstimatedCost).toInt()};
    int actualCost{columnData(record, Column::ActualCost).toInt()};
    QStringList tagNames{columnData(record, Column::Tags)
                             .toString()
                             .split(",", QString::SkipEmptyParts)};
    std::list<entities::Tag> tags;
    std::transform(tagNames.cbegin(),
                   tagNames.cend(),
                   std::back_inserter(tags),
                   [](const auto& tag) { return Tag{tag.toStdString()}; });
    bool finished{columnData(record, Column::Completed).toBool()};
    QDateTime qLastModified{
        columnData(record, Column::LastModified).toDateTime()};
    dw::DateTime lastModified
        = utils::DateTimeConverter::dateTime(qLastModified);
    return Task{name,
                estimatedCost,
                actualCost,
                uuid,
                std::move(tags),
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

} // namespace sprint_timer::storage::qt_storage_impl
