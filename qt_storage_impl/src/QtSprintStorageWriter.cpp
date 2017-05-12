/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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
#include "qt_storage_impl/QtSprintStorageWriter.h"
#include "qt_storage_impl/Database.h"
#include "utils/DateTimeConverter.h"


QtSprintStorageWriter::QtSprintStorageWriter(DBService& dbService)
    : dbService{dbService}
{
    addQueryId = dbService.prepare(
        QString{"INSERT INTO %1(%2, %3, %4, %5) "
                "VALUES(:todo_uuid, :startTime, :finishTime, :uuid);"}
            .arg(SprintView::name)
            .arg(SprintTable::Columns::taskUuid)
            .arg(SprintTable::Columns::startTime)
            .arg(SprintTable::Columns::finishTime)
            .arg(SprintTable::Columns::uuid));
    removeQueryId
        = dbService.prepare(QString{"DELETE FROM %1 WHERE %2 = (:uuid);"}
                                .arg(SprintView::name)
                                .arg(SprintTable::Columns::uuid));
}

void QtSprintStorageWriter::save(const Sprint& sprint)
{
    QDateTime startTime
        = DateTimeConverter::qDateTime(sprint.timeSpan().start());
    QDateTime finishTime
        = DateTimeConverter::qDateTime(sprint.timeSpan().finish());
    dbService.bind(addQueryId,
                   ":todo_uuid",
                   QVariant(QString::fromStdString(sprint.taskUuid())));
    dbService.bind(addQueryId, ":startTime", QVariant(startTime));
    dbService.bind(addQueryId, ":finishTime", QVariant(finishTime));
    dbService.bind(
        addQueryId, ":uuid", QVariant(QString::fromStdString(sprint.uuid())));
    dbService.executePrepared(addQueryId);
}

void QtSprintStorageWriter::remove(const Sprint& sprint)
{
    dbService.bind(removeQueryId,
                   ":uuid",
                   QVariant(QString::fromStdString(sprint.uuid())));
    dbService.executePrepared(removeQueryId);
}
