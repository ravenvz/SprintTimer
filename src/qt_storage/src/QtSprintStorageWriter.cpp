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

QtSprintStorageWriter::QtSprintStorageWriter(const QString& connectionName_)
    : connectionName{connectionName_}
{
    saveSprintQuery = tryPrepare(
        connectionName,
        QString{"INSERT INTO %1(%2, %3, %4, %5) "
                "VALUES(:todo_uuid, :startTime, :finishTime, :uuid);"}
            .arg(SprintView::name)
            .arg(SprintTable::Columns::taskUuid)
            .arg(SprintTable::Columns::startTime)
            .arg(SprintTable::Columns::finishTime)
            .arg(SprintTable::Columns::uuid));
    deleteSprintQuery = tryPrepare(connectionName,
                                   QString{"DELETE FROM %1 WHERE %2 = (:uuid);"}
                                       .arg(SprintView::name)
                                       .arg(SprintTable::Columns::uuid));
}

void QtSprintStorageWriter::save(const entities::Sprint& sprint)
{
    using storage::utils::DateTimeConverter;
    const QDateTime startTime =
        DateTimeConverter::qDateTime(sprint.timeSpan().start());
    const QDateTime finishTime =
        DateTimeConverter::qDateTime(sprint.timeSpan().finish());
    saveSprintQuery.bindValue(
        ":todo_uuid", QVariant(QString::fromStdString(sprint.taskUuid())));
    saveSprintQuery.bindValue(":startTime", QVariant(startTime));
    saveSprintQuery.bindValue(":finishTime", QVariant(finishTime));
    saveSprintQuery.bindValue(":uuid",
                              QVariant(QString::fromStdString(sprint.uuid())));
    tryExecute(saveSprintQuery);
}

void QtSprintStorageWriter::save(const std::vector<entities::Sprint>& sprints)
{
    TransactionGuard guard{connectionName};
    for (const auto& sprint : sprints)
        save(sprint);
    guard.commit();
}

void QtSprintStorageWriter::remove(const entities::Sprint& sprint)
{
    deleteSprintQuery.bindValue(
        ":uuid", QVariant(QString::fromStdString(sprint.uuid())));
    tryExecute(deleteSprintQuery);
}

void QtSprintStorageWriter::remove(const std::vector<entities::Sprint>& sprints)
{
    TransactionGuard guard{connectionName};
    for (const auto& sprint : sprints)
        remove(sprint);
    guard.commit();
}

} // namespace sprint_timer::storage::qt_storage
