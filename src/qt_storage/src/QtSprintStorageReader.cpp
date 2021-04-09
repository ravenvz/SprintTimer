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
#include "qt_storage/QtSprintStorageReader.h"
#include "qt_storage/DatabaseDescription.h"
#include "qt_storage/utils/DateTimeConverter.h"
#include "qt_storage/utils/QueryUtils.h"

namespace {

enum class Columns {
    Id = 0,
    TodoUuid,
    Name,
    Tags,
    StartTime,
    FinishTime,
    Uuid,
};

QVariant columnData(const QSqlRecord& record, Columns column);

std::vector<sprint_timer::entities::Sprint> sprintsFromQuery(QSqlQuery& query);

sprint_timer::entities::Sprint sprintFromQSqlRecord(const QSqlRecord& record);

} // namespace

namespace sprint_timer::storage::qt_storage {

using namespace dw;
using namespace storage::utils;
using namespace entities;

QtSprintStorageReader::QtSprintStorageReader(const QString& connectionName_)
    : connectionName{connectionName_}
{
    findByDateRangeQuery =
        tryPrepare(connectionName,
                   QString{"SELECT %1, %2, %3, %4, %5, %6, %7 "
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

    findByTaskUuidQuery =
        tryPrepare(connectionName,
                   QString{"SELECT %1, %2, %3, %4, %5, %6, %7 "
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
}

std::vector<Sprint>
QtSprintStorageReader::findByDateRange(const dw::DateRange& dateRange)
{
    findByDateRangeQuery.bindValue(":startTime",
                                   QVariant(QString::fromStdString(to_string(
                                       dateRange.start(), "yyyy-MM-dd"))));
    findByDateRangeQuery.bindValue(":finishTime",
                                   QVariant(QString::fromStdString(to_string(
                                       dateRange.finish(), "yyyy-MM-dd"))));

    tryExecute(findByDateRangeQuery);

    return sprintsFromQuery(findByDateRangeQuery);
}

std::vector<Sprint>
QtSprintStorageReader::findByTaskUuid(const std::string& uuid)
{
    findByTaskUuidQuery.bindValue(":taskUuid",
                                  QVariant{QString::fromStdString(uuid)});
    tryExecute(findByTaskUuidQuery);
    return sprintsFromQuery(findByTaskUuidQuery);
}

} // namespace sprint_timer::storage::qt_storage

namespace {

std::vector<sprint_timer::entities::Sprint> sprintsFromQuery(QSqlQuery& query)
{
    using namespace sprint_timer::storage::qt_storage;
    const auto records = copyAllRecords(query);
    std::vector<sprint_timer::entities::Sprint> sprints;
    sprints.reserve(records.size());
    std::transform(records.cbegin(),
                   records.cend(),
                   std::back_inserter(sprints),
                   sprintFromQSqlRecord);
    return sprints;
}

sprint_timer::entities::Sprint sprintFromQSqlRecord(const QSqlRecord& record)
{
    using sprint_timer::entities::Sprint;
    using sprint_timer::entities::Tag;
    using sprint_timer::storage::utils::DateTimeConverter;
    QString name{columnData(record, Columns::Name).toString()};
    QDateTime start = columnData(record, Columns::StartTime).toDateTime();
    QDateTime finish = columnData(record, Columns::FinishTime).toDateTime();
    const dw::DateTimeRange timeSpan{DateTimeConverter::dateTime(start),
                                     DateTimeConverter::dateTime(finish)};
    std::string uuid =
        columnData(record, Columns::Uuid).toString().toStdString();
    QStringList tagNames{columnData(record, Columns::Tags)
                             .toString()
                             .split(",", Qt::SkipEmptyParts)};
    std::string taskUuid =
        columnData(record, Columns::TodoUuid).toString().toStdString();
    std::list<Tag> tags;
    std::transform(tagNames.cbegin(),
                   tagNames.cend(),
                   std::back_inserter(tags),
                   [](const auto& nm) { return Tag{nm.toStdString()}; });
    return Sprint{name.toStdString(), timeSpan, tags, uuid, taskUuid};
}

QVariant columnData(const QSqlRecord& record, Columns column)
{
    return record.value(static_cast<int>(column));
}

} // namespace
