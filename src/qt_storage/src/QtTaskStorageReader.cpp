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
#include "api/GatewayException.h"
#include "core/Note.h"
#include "core/utils/Algutils.h"
#include "core/utils/StringUtils.h"
#include "qt_storage/DatabaseDescription.h"
#include "qt_storage/utils/DateTimeConverter.h"
#include "qt_storage/utils/QueryUtils.h"
#include <fstream>

namespace {

using sprint_timer::Task;
using sprint_timer::storage::qt_storage::maybeBool;
using sprint_timer::storage::qt_storage::maybeDateTime;
using sprint_timer::storage::qt_storage::maybeInt;
using sprint_timer::storage::qt_storage::maybeString;

enum class TaskColumn {
    Uuid = 0,
    Name,
    EstimatedCost,
    Completed,
    Tags,
    LastModified,
    StartTime,
    FinishTime,
    Note,
    Start,
    Due,
    Reminder,
    Recurrence,
    Type,
    Id
};

enum class TaskMetadataColumn { Uuid = 0 };

enum class TagColumn { Id, Name };

struct TaskMetadata {
    std::string uuid{};
};

template <class CharT, class Traits>
auto operator<<(std::basic_ostream<CharT, Traits>& os, const TaskMetadata& data)
    -> std::basic_ostream<CharT, Traits>&;

using TaskMetadataTree = sprint_timer::Tree<std::string, TaskMetadata>;

auto tasksFromQuery(QSqlQuery& query) -> std::vector<Task>;

auto taskFromRecords(auto first, auto last) -> Task;

auto tagsFromQuery(QSqlQuery& query) -> std::vector<std::string>;

auto tagFromRecord(const QSqlRecord& record) -> std::string;

auto readTimeframe(
    const QSqlRecord& record, int start, int due, int reminder, int recurrence)
    -> std::optional<sprint_timer::TaskTimeframe>;

auto readTreeMetadata(const QString& connectionName) -> TaskMetadataTree;

auto readTags(const QSqlRecord& record, int column)
    -> std::vector<sprint_timer::Tag>;

auto to_int(TaskColumn taskColumn) -> int;

auto to_int(TaskMetadataColumn taskNodeColumn) -> int;

auto to_int(TagColumn tagColumn) -> int;

auto convertDateTime(const std::optional<QDateTime>& dateTime)
    -> std::optional<dw::DateTime>;

auto makeTaskType(int32_t numeric) -> sprint_timer::TaskType;

} // namespace

namespace sprint_timer::storage::qt_storage {

QtTaskStorageReader::QtTaskStorageReader(QString connectionName_)
    : connectionName{std::move(connectionName_)}
{
    finishedTasksQuery =
        tryPrepare(connectionName,
                   QString{"SELECT %1, %2, %3, %4, %5, %6, %7, %8, %9, %10, "
                           "%11, %12, %13, %14 "
                           "FROM %15 "
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
                       .arg(NotesTable::Columns::text)
                       .arg(TaskTimeframeTable::Columns::start)
                       .arg(TaskTimeframeTable::Columns::due)
                       .arg(TaskTimeframeTable::Columns::reminder)
                       .arg(TaskTimeframeTable::Columns::recurrence)
                       .arg(TaskTable::Columns::type)
                       .arg(AdvTaskView::name));

    allTasksQuery = tryPrepare(
        connectionName,
        QString{"SELECT %1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, %12, "
                "%13, %14 "
                "FROM %15 "
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
            .arg(NotesTable::Columns::text)
            .arg(TaskTimeframeTable::Columns::start)
            .arg(TaskTimeframeTable::Columns::due)
            .arg(TaskTimeframeTable::Columns::reminder)
            .arg(TaskTimeframeTable::Columns::recurrence)
            .arg(TaskTable::Columns::type)
            .arg(AdvTaskView::name));

    findByUuidQuery =
        tryPrepare(connectionName,
                   QString{"SELECT %1, %2, %3, %4, %5, %6, %7, %8, %9, %10, "
                           "%11, %12, %13, %14 "
                           "FROM %15 "
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
                       .arg(NotesTable::Columns::text)
                       .arg(TaskTimeframeTable::Columns::start)
                       .arg(TaskTimeframeTable::Columns::due)
                       .arg(TaskTimeframeTable::Columns::reminder)
                       .arg(TaskTimeframeTable::Columns::recurrence)
                       .arg(TaskTable::Columns::type)
                       .arg(AdvTaskView::name));
}

std::vector<Task> QtTaskStorageReader::unfinishedTasks()
{
    QSqlQuery query{QSqlDatabase::database(connectionName)};
    tryExecute(query,
               QString{"SELECT %1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, "
                       "%12, %13, %14 "
                       "FROM %15 "
                       "WHERE %4 = 0 OR %6 > DATETIME('now', '-1 day') "
                       "ORDER BY %6, %1;"}
                   .arg(TaskTable::Columns::uuid)
                   .arg(TaskTable::Columns::name)
                   .arg(TaskTable::Columns::estimatedCost)
                   .arg(TaskTable::Columns::completed)
                   .arg(AdvTaskView::Aliases::tags)
                   .arg(TaskTable::Columns::lastModified)
                   .arg(SprintTable::Columns::startTime)
                   .arg(SprintTable::Columns::finishTime)
                   .arg(NotesTable::Columns::text)
                   .arg(TaskTimeframeTable::Columns::start)
                   .arg(TaskTimeframeTable::Columns::due)
                   .arg(TaskTimeframeTable::Columns::reminder)
                   .arg(TaskTimeframeTable::Columns::recurrence)
                   .arg(TaskTable::Columns::type)
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

std::vector<Task> QtTaskStorageReader::findByUuid(const std::string& uuid)
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
               QString{"SELECT %1, %2, %3, %4, %5, %6, %7, %8, %9, %10, %11, "
                       "%12, %13, %14 "
                       "FROM %15 "
                       "WHERE %1 IN (%16);"}
                   .arg(TaskTable::Columns::uuid)
                   .arg(TaskTable::Columns::name)
                   .arg(TaskTable::Columns::estimatedCost)
                   .arg(TaskTable::Columns::completed)
                   .arg(AdvTaskView::Aliases::tags)
                   .arg(TaskTable::Columns::lastModified)
                   .arg(SprintTable::Columns::startTime)
                   .arg(SprintTable::Columns::finishTime)
                   .arg(NotesTable::Columns::text)
                   .arg(TaskTimeframeTable::Columns::start)
                   .arg(TaskTimeframeTable::Columns::due)
                   .arg(TaskTimeframeTable::Columns::reminder)
                   .arg(TaskTimeframeTable::Columns::recurrence)
                   .arg(TaskTable::Columns::type)
                   .arg(AdvTaskView::name)
                   .arg(QString::fromStdString(us)));
    return tasksFromQuery(query);
}

auto QtTaskStorageReader::taskTree() -> TaskTree
{
    const auto metaTree = readTreeMetadata(connectionName);

    std::vector<std::string> uuids;

    std::ranges::copy(metaTree.keys(), std::back_inserter(uuids));

    auto tasks = findMatching(uuids);
    std::unordered_map<std::string, Task> taskMap;
    for (auto& task : tasks) {
        taskMap.insert({task.uuid(), std::move(task)});
    }

    auto combine = [&](const auto& payload) {
        if (auto it = taskMap.find(payload.uuid); it != taskMap.cend()) {
            return it->second;
        }
        throw api::GatewayException{std::string{"Error reading task tree: "} +
                                    std::string{"task with uuid = "} +
                                    payload.uuid +
                                    std::string{" cannot be found."}};
    };

    return metaTree.mapped(combine);
}

} // namespace sprint_timer::storage::qt_storage

namespace {

using sprint_timer::Sprint;
using sprint_timer::Tag;
using sprint_timer::Task;

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const TaskMetadata& data)
{
    os << "TaskMetadata{";
    os << "uuid: " << data.uuid << "}";
    return os;
}

auto readTreeMetadata(const QString& connectionName) -> TaskMetadataTree
{
    using namespace sprint_timer::storage::qt_storage;
    using sprint_timer::utils::transform;

    using entry_t = TaskMetadataTree::entry_t;

    QSqlQuery query(QSqlDatabase::database(connectionName));

    tryExecute(query,
               QString{"SELECT %1 from %2"}
                   .arg(TaskTreeTable::Columns::task_uuid)
                   .arg(TaskTreeTable::name));

    auto read_metadata =
        [](const auto& record) -> std::optional<TaskMetadataTree::entry_t> {
        auto uuid = maybeString(record, to_int(TaskMetadataColumn::Uuid));
        return transform(uuid, [](const auto& id) {
            return TaskMetadataTree::entry_t{id, id};
        });
    };

    const auto records = copyAllRecords(query);
    if (records.empty()) {
        return TaskMetadataTree{};
    }

    std::vector<std::optional<entry_t>> flattenedTree;
    flattenedTree.reserve(records.size());

    std::ranges::transform(
        records, std::back_inserter(flattenedTree), read_metadata);

    return TaskMetadataTree::unflatten(flattenedTree);
}

auto tasksFromQuery(QSqlQuery& query) -> std::vector<Task>
{
    const auto records =
        sprint_timer::storage::qt_storage::copyAllRecords(query);
    std::vector<Task> tasks;
    if (records.empty()) {
        return tasks;
    }

    for (auto it = records.cbegin(); it != records.cend();) {
        QVariant uuid = it->value(to_int(TaskColumn::Uuid));
        auto next = std::find_if(it, records.cend(), [&](const auto& record) {
            return record.value(to_int(TaskColumn::Uuid)) != uuid;
        });
        tasks.push_back(taskFromRecords(it, next));
        it = next;
    }

    return tasks;
}

auto taskFromRecords(auto first, auto last) -> Task
{
    const sprint_timer::storage::utils::DateTimeConverter dateTimeConverter;
    using namespace sprint_timer::storage::qt_storage;
    using sprint_timer::utils::transform;

    const auto name = maybeString(*first, to_int(TaskColumn::Name));
    const auto uuid = maybeString(*first, to_int(TaskColumn::Uuid));
    const auto estimatedCost =
        maybeInt(*first, to_int(TaskColumn::EstimatedCost));
    const auto tags = readTags(*first, to_int(TaskColumn::Tags));
    const auto finished = maybeBool(*first, to_int(TaskColumn::Completed));
    const auto lastModified = convertDateTime(
        maybeDateTime(*first, to_int(TaskColumn::LastModified)));
    const auto type = maybeInt(*first, to_int(TaskColumn::Type));
    const auto notes =
        transform(maybeString(*first, to_int(TaskColumn::Note)),
                  [](const auto& str) { return sprint_timer::Note{str}; });
    const auto timeFrame = readTimeframe(*first,
                                         to_int(TaskColumn::Start),
                                         to_int(TaskColumn::Due),
                                         to_int(TaskColumn::Reminder),
                                         to_int(TaskColumn::Recurrence));

    std::vector<Sprint> sprints;
    for (; first != last; ++first) {
        const QDateTime startTime{
            first->value(to_int(TaskColumn::StartTime)).toDateTime()};
        // If task has no sprints, field would be an empty string thus
        // resulting an invalid QDateTime when parsing. So we break here
        // and task will have empty sprints.
        if (!startTime.isValid()) {
            break;
        }
        const QDateTime finishTime{
            first->value(to_int(TaskColumn::FinishTime)).toDateTime()};
        sprints.push_back(Sprint{dw::DateTimeRange{
            dateTimeConverter(startTime), dateTimeConverter(finishTime)}});
    }

    try {
        return Task{name.value(),
                    estimatedCost.value(),
                    sprints,
                    uuid.value(),
                    tags,
                    finished.value(),
                    lastModified.value(),
                    transform(type,
                              [](const auto& integralType) {
                                  return makeTaskType(integralType);
                              })
                        .value(),
                    notes,
                    timeFrame.value()};
    }
    catch (std::exception& exc) {
        throw sprint_timer::api::GatewayException{"Malformed task record."};
    }
};

auto readTags(const QSqlRecord& record, int column)
    -> std::vector<sprint_timer::Tag>
{
    const QStringList tagNames{
        record.value(column).toString().split(",", Qt::SkipEmptyParts)};
    std::vector<Tag> tags;
    std::ranges::transform(
        tagNames, std::back_inserter(tags), [](const auto& tag) {
            return Tag{tag.toStdString()};
        });

    return tags;
}

auto readTimeframe(const QSqlRecord& record,
                   int start,
                   int due,
                   int reminder,
                   int recurrence) -> std::optional<sprint_timer::TaskTimeframe>
{
    using namespace sprint_timer::storage::qt_storage;
    using sprint_timer::utils::transform;

    const auto startV = convertDateTime(maybeDateTime(record, start));
    const auto dueV = convertDateTime(maybeDateTime(record, due));
    const auto reminderV = convertDateTime(maybeDateTime(record, reminder));

    std::optional<sprint_timer::Recurrence> recurrenceV;
    if (auto recurr = record.value(recurrence); !recurr.isNull()) {
        recurrenceV = sprint_timer::Recurrence{recurr.toString().toStdString()};
    }

    std::optional<sprint_timer::TaskTimeframe> timeFrame;
    return sprint_timer::TaskTimeframe{
        startV.value_or(dw::current_date_time_local()),
        dueV,
        reminderV,
        recurrenceV};
}

std::vector<std::string> tagsFromQuery(QSqlQuery& query)
{
    using namespace sprint_timer::storage::qt_storage;
    const auto records = copyAllRecords(query);
    std::vector<std::string> tags;
    tags.reserve(records.size());
    std::ranges::transform(records, std::back_inserter(tags), tagFromRecord);
    return tags;
}

std::string tagFromRecord(const QSqlRecord& record)
{
    return record.value(to_int(TagColumn::Name)).toString().toStdString();
}

auto to_int(TaskColumn taskColumn) -> int
{
    return static_cast<int>(taskColumn);
}

auto to_int(TaskMetadataColumn taskNodeColumn) -> int
{
    return static_cast<int>(taskNodeColumn);
}

auto to_int(TagColumn tagColumn) -> int { return static_cast<int>(tagColumn); }

auto convertDateTime(const std::optional<QDateTime>& dateTime)
    -> std::optional<dw::DateTime>
{
    const sprint_timer::storage::utils::DateTimeConverter dateTimeConverter;
    using namespace sprint_timer::storage::qt_storage;
    using sprint_timer::utils::transform;

    return transform(dateTime,
                     [&](const auto& dt) { return dateTimeConverter(dt); });
}

auto makeTaskType(int32_t numeric) -> sprint_timer::TaskType
{
    using sprint_timer::TaskType;
    switch (numeric) {
    case static_cast<int32_t>(TaskType::Project):
    case static_cast<int32_t>(TaskType::Folder):
    case static_cast<int32_t>(TaskType::Regular):
        return static_cast<TaskType>(numeric);
    default:
        throw std::runtime_error{"TaskTypeDTO enum is out of range"};
    }
}
} // namespace
