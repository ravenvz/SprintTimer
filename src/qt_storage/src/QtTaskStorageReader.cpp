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
#include "qt_storage/QtTaskStorageReader.h"
#include "core/utils/StringUtils.h"
#include "qt_storage/DatabaseDescription.h"
#include "qt_storage/utils/DateTimeConverter.h"
#include "qt_storage/utils/QueryUtils.h"

#include <QSqlError>

namespace {

using sprint_timer::Task;

enum class Column {
    Uuid = 0,
    Name,
    EstimatedCost,
    Completed,
    Tags,
    LastModified,
    StartTime,
    FinishTime,
};

enum class TagColumn { Id, Name };

std::vector<Task> tasksFromQuery(QSqlQuery& query);

std::vector<std::string> tagsFromQuery(QSqlQuery& query);

std::string tagFromRecord(const QSqlRecord& record);

QVariant columnData(const QSqlRecord& record, Column column);

} // namespace

namespace sprint_timer::storage::qt_storage {

QtTaskStorageReader::QtTaskStorageReader(QString connectionName_)
    : connectionName{std::move(connectionName_)}
{
    finishedTasksQuery =
        tryPrepare(connectionName,
                   QString{"SELECT %1, %2, %3, %4, %5, %6, %7, %8 "
                           "FROM %9 "
                           "WHERE %4 = 1 AND DATE(%6) >= (:start_date) "
                           "AND DATE(%6) <= (:end_date) "
                           "ORDER BY %6, %1;"}
                       .arg(TaskTable::Columns::uuid)
                       .arg(TaskTable::Columns::name)
                       .arg(TaskTable::Columns::estimatedCost)
                       .arg(TaskTable::Columns::completed)
                       .arg(TasksView::Aliases::tags)
                       .arg(TaskTable::Columns::lastModified)
                       .arg(SprintTable::Columns::startTime)
                       .arg(SprintTable::Columns::finishTime)
                       .arg(AdvTaskView::name));

    allTasksQuery = tryPrepare(
        connectionName,
        QString{"SELECT %1, %2, %3, %4, %5, %6, %7, %8 "
                "FROM %9 "
                "WHERE DATE(%6) >= (:start_date) AND DATE(%6) <= (:end_date) "
                "ORDER BY %6, %1;"}
            .arg(TaskTable::Columns::uuid)
            .arg(TaskTable::Columns::name)
            .arg(TaskTable::Columns::estimatedCost)
            .arg(TaskTable::Columns::completed)
            .arg(TasksView::Aliases::tags)
            .arg(TaskTable::Columns::lastModified)
            .arg(SprintTable::Columns::startTime)
            .arg(SprintTable::Columns::finishTime)
            .arg(AdvTaskView::name));

    findByUuidQuery =
        tryPrepare(connectionName,
                   QString{"SELECT %1, %2, %3, %4, %5, %6, %7, %8 "
                           "FROM %9 "
                           "WHERE %1 = (:uuid) "
                           "ORDER BY %6, %1;"}
                       .arg(TaskTable::Columns::uuid)
                       .arg(TaskTable::Columns::name)
                       .arg(TaskTable::Columns::estimatedCost)
                       .arg(TaskTable::Columns::completed)
                       .arg(TasksView::Aliases::tags)
                       .arg(TaskTable::Columns::lastModified)
                       .arg(SprintTable::Columns::startTime)
                       .arg(SprintTable::Columns::finishTime)
                       .arg(AdvTaskView::name));

    findMatchingQuery =
        tryPrepare(connectionName,
                   QString{"SELECT %1, %2, %3, %4, %5, %6, %7, %8 "
                           "FROM %9 "
                           "WHERE %1 IN ((:uuids_list));"}
                       .arg(TaskTable::Columns::uuid)
                       .arg(TaskTable::Columns::name)
                       .arg(TaskTable::Columns::estimatedCost)
                       .arg(TaskTable::Columns::completed)
                       .arg(TasksView::Aliases::tags)
                       .arg(TaskTable::Columns::lastModified)
                       .arg(SprintTable::Columns::startTime)
                       .arg(SprintTable::Columns::finishTime)
                       .arg(AdvTaskView::name));
}

std::vector<Task> QtTaskStorageReader::unfinishedTasks()
{
    QSqlQuery query{QSqlDatabase::database(connectionName)};
    tryExecute(query,
               QString{"SELECT %1, %2, %3, %4, %5, %6, %7, %8 "
                       "FROM %9 "
                       "WHERE %4 = 0 OR %6 > DATETIME('now', '-1 day') "
                       "ORDER BY %6, %1;"}
                   .arg(TaskTable::Columns::uuid)
                   .arg(TaskTable::Columns::name)
                   .arg(TaskTable::Columns::estimatedCost)
                   .arg(TaskTable::Columns::completed)
                   .arg(TasksView::Aliases::tags)
                   .arg(TaskTable::Columns::lastModified)
                   .arg(SprintTable::Columns::startTime)
                   .arg(SprintTable::Columns::finishTime)
                   .arg(AdvTaskView::name));
    return tasksFromQuery(query);
}

std::vector<Task>
QtTaskStorageReader::finishedTasks(const dw::DateRange& dateRange)
{
    finishedTasksQuery.bindValue(":start_date",
                                 QVariant(QString::fromStdString(dw::to_string(
                                     dateRange.start(), "yyyy-MM-dd"))));
    finishedTasksQuery.bindValue(":end_date",
                                 QVariant(QString::fromStdString(dw::to_string(
                                     dateRange.finish(), "yyyy-MM-dd"))));
    tryExecute(finishedTasksQuery);
    return tasksFromQuery(finishedTasksQuery);
}

std::vector<Task> QtTaskStorageReader::allTasks(const dw::DateRange& dateRange)
{
    allTasksQuery.bindValue(":start_date",
                            QVariant(QString::fromStdString(dw::to_string(
                                dateRange.start(), "yyyy-MM-dd"))));
    allTasksQuery.bindValue(":end_date",
                            QVariant(QString::fromStdString(dw::to_string(
                                dateRange.finish(), "yyyy-MM-dd"))));
    tryExecute(allTasksQuery);
    return tasksFromQuery(allTasksQuery);
}

std::vector<std::string> QtTaskStorageReader::allTags()
{
    QSqlQuery query{QSqlDatabase::database(connectionName)};
    tryExecute(query,
               QString{"SELECT %1, %2 FROM %3 "
                       "ORDER BY %2;"}
                   .arg(TagTable::Columns::id)
                   .arg(TagTable::Columns::name)
                   .arg(TagTable::name));

    return tagsFromQuery(query);
}

std::vector<Task>
QtTaskStorageReader::findByUuid(const std::string& uuid)
{
    findByUuidQuery.bindValue(":uuid", QVariant(QString::fromStdString(uuid)));
    tryExecute(findByUuidQuery);
    return tasksFromQuery(findByUuidQuery);
}

std::vector<Task>
QtTaskStorageReader::findMatching(std::span<const std::string> uuids)
{
    const auto us = sprint_timer::utils::transformJoin(
        cbegin(uuids), cend(uuids), ",", [](const auto& id) {
            return "'" + std::string{id} + "'";
        });
    QSqlQuery query(QSqlDatabase::database(connectionName));
    tryExecute(query,
               QString{"SELECT %1, %2, %3, %4, %5, %6, %7, %8 "
                       "FROM %9 "
                       "WHERE %1 IN (%10);"}
                   .arg(TaskTable::Columns::uuid)
                   .arg(TaskTable::Columns::name)
                   .arg(TaskTable::Columns::estimatedCost)
                   .arg(TaskTable::Columns::completed)
                   .arg(TasksView::Aliases::tags)
                   .arg(TaskTable::Columns::lastModified)
                   .arg(SprintTable::Columns::startTime)
                   .arg(SprintTable::Columns::finishTime)
                   .arg(AdvTaskView::name)
                   .arg(QString::fromStdString(us)));

    return tasksFromQuery(query);
}

} // namespace sprint_timer::storage::qt_storage

namespace {

using sprint_timer::Sprint;
using sprint_timer::SprintRecord;
using sprint_timer::Tag;
using sprint_timer::Task;

auto taskFromRecords(auto first, auto last) -> Task
{
    using sprint_timer::storage::utils::DateTimeConverter;

    const std::string name{
        columnData(*first, Column::Name).toString().toStdString()};
    const std::string uuid{
        columnData(*first, Column::Uuid).toString().toStdString()};
    const int estimatedCost{columnData(*first, Column::EstimatedCost).toInt()};
    const QStringList tagNames{columnData(*first, Column::Tags)
                                   .toString()
                                   .split(",", Qt::SkipEmptyParts)};
    std::vector<Tag> tags;
    std::ranges::transform(
        tagNames, std::back_inserter(tags), [](const auto& tag) {
            return Tag{tag.toStdString()};
        });
    const bool finished{columnData(*first, Column::Completed).toBool()};
    const QDateTime qLastModified{
        columnData(*first, Column::LastModified).toDateTime()};
    const dw::DateTime lastModified =
        sprint_timer::storage::utils::DateTimeConverter::dateTime(
            qLastModified);

    std::vector<Sprint> sprints;
    for (; first != last; ++first) {
        const QDateTime startTime{
            columnData(*first, Column::StartTime).toDateTime()};
        // If task has no sprints, field would be an empty string thus
        // resulting an invalid QDateTime when parsing. So we break here
        // and task will have empty sprints.
        if (!startTime.isValid()) {
            break;
        }
        const QDateTime finishTime{
            columnData(*first, Column::FinishTime).toDateTime()};
        sprints.push_back(
            Sprint{dw::DateTimeRange{DateTimeConverter::dateTime(startTime),
                                     DateTimeConverter::dateTime(finishTime)}});
    }

    return Task{
        name, estimatedCost, sprints, uuid, tags, finished, lastModified};
};

std::vector<Task> tasksFromQuery(QSqlQuery& query)
{
    const auto records =
        sprint_timer::storage::qt_storage::copyAllRecords(query);
    std::vector<Task> tasks;
    if (records.empty()) {
        return tasks;
    }

    for (auto it = records.cbegin(); it != records.cend();) {
        QVariant uuid = columnData(*it, Column::Uuid);
        auto next = std::find_if(it, records.cend(), [&](const auto& record) {
            return columnData(record, Column::Uuid) != uuid;
        });
        Task task = taskFromRecords(it, next);
        tasks.push_back(task);
        it = next;
    }

    return tasks;
}

std::vector<std::string> tagsFromQuery(QSqlQuery& query)
{
    using namespace sprint_timer::storage::qt_storage;
    const auto records = copyAllRecords(query);
    std::vector<std::string> tags;
    tags.reserve(records.size());
    std::transform(records.cbegin(),
                   records.cend(),
                   std::back_inserter(tags),
                   tagFromRecord);
    while (query.next())
        tags.push_back(tagFromRecord(query.record()));
    return tags;
}

std::string tagFromRecord(const QSqlRecord& record)
{
    return record.value(static_cast<int>(TagColumn::Name))
        .toString()
        .toStdString();
}

QVariant columnData(const QSqlRecord& record, Column column)
{
    return record.value(static_cast<int>(column));
}

} // namespace
