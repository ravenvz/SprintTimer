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
#include "qt_storage/QtSprintStorageWriter.h"
#include "qt_storage/DatabaseDescription.h"
#include "qt_storage/TransactionGuard.h"
#include "qt_storage/utils/DateTimeConverter.h"
#include "qt_storage/utils/QueryUtils.h"

namespace sprint_timer::storage::qt_storage {

using namespace qt_storage;

QtSprintStorageWriter::QtSprintStorageWriter(QString connectionName_)
    : connectionName{std::move(connectionName_)}
{
    saveSprintQuery =
        tryPrepare(connectionName,
                   QString{"INSERT INTO %1(%2, %3, %4) "
                           "VALUES("
                           "(SELECT %5 FROM %6 WHERE %7 = :todo_uuid), "
                           ":startTime, :finishTime);"}
                       .arg(SprintView::name)
                       .arg(SprintView::Aliases::taskid)
                       .arg(SprintTable::Columns::startTime)
                       .arg(SprintTable::Columns::finishTime)
                       .arg(TaskTable::Columns::id)
                       .arg(TaskTable::name)
                       .arg(TaskTable::Columns::uuid));
    deleteSprintQuery = tryPrepare(connectionName,
                                   QString{"UPDATE %1 set %2 = 1 "
                                           "WHERE %3 = (:startTime);"}
                                       .arg(SprintTable::name)
                                       .arg(SprintTable::Columns::deleted)
                                       .arg(SprintTable::Columns::startTime));
    restoreSprintQuery = tryPrepare(connectionName,
                                    QString{"UPDATE %1 set %2 = 0 "
                                            "WHERE %3 = (:startTime);"}
                                        .arg(SprintTable::name)
                                        .arg(SprintTable::Columns::deleted)
                                        .arg(SprintTable::Columns::startTime));
}

void QtSprintStorageWriter::save(const std::string& taskUuid,
                                 const std::vector<Sprint>& sprints)
{
    TransactionGuard guard{connectionName};
    for (const auto& sprint : sprints) {
        save(taskUuid, sprint);
    }
    guard.commit();
}

void QtSprintStorageWriter::save(const std::string& taskUuid,
                                 const Sprint& sprint)
{
    using storage::utils::DateTimeConverter;
    const QDateTime startTime =
        DateTimeConverter::qDateTime(sprint.timeSpan().start());
    const QDateTime finishTime =
        DateTimeConverter::qDateTime(sprint.timeSpan().finish());
    saveSprintQuery.bindValue(":todo_uuid",
                              QVariant(QString::fromStdString(taskUuid)));
    saveSprintQuery.bindValue(":startTime", QVariant(startTime));
    saveSprintQuery.bindValue(":finishTime", QVariant(finishTime));
    tryExecute(saveSprintQuery);
}

void QtSprintStorageWriter::remove(const Sprint& sprint)
{
    using storage::utils::DateTimeConverter;
    const QDateTime startTime =
        DateTimeConverter::qDateTime(sprint.timeSpan().start());
    deleteSprintQuery.bindValue(":startTime", QVariant(startTime));
    tryExecute(deleteSprintQuery);
}

void QtSprintStorageWriter::restore(const Sprint& sprint)
{
    using storage::utils::DateTimeConverter;
    const QDateTime startTime =
        DateTimeConverter::qDateTime(sprint.timeSpan().start());
    restoreSprintQuery.bindValue(":startTime", QVariant(startTime));
    tryExecute(restoreSprintQuery);
}

void QtSprintStorageWriter::remove(const std::vector<Sprint>& sprints)
{
    TransactionGuard guard{connectionName};
    for (const auto& sprint : sprints) {
        remove(sprint);
    }
    guard.commit();
}

} // namespace sprint_timer::storage::qt_storage
