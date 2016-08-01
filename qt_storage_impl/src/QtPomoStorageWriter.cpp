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
#include "qt_storage_impl/QtPomoStorageWriter.h"
#include "utils/DateTimeConverter.h"


QtPomoStorageWriter::QtPomoStorageWriter(DBService& dbService)
    : dbService{dbService}
{
    addQueryId = dbService.prepare(
        "insert into pomodoro_view(todo_uuid, start_time, finish_time, uuid) "
        "values(:todo_uuid, :startTime, :finishTime, :uuid);");
    removeQueryId
        = dbService.prepare("delete from pomodoro_view where uuid = (:uuid);");
}

void QtPomoStorageWriter::save(const Pomodoro& pomodoro)
{
    QDateTime startTime
        = DateTimeConverter::qDateTime(pomodoro.timeSpan().startTime);
    QDateTime finishTime
        = DateTimeConverter::qDateTime(pomodoro.timeSpan().finishTime);
    dbService.bind(addQueryId,
                   ":todo_uuid",
                   QVariant(QString::fromStdString(pomodoro.taskUuid())));
    dbService.bind(addQueryId, ":startTime", QVariant(startTime));
    dbService.bind(addQueryId, ":finishTime", QVariant(finishTime));
    dbService.bind(
        addQueryId, ":uuid", QVariant(QString::fromStdString(pomodoro.uuid())));
    dbService.executePrepared(addQueryId);
}

void QtPomoStorageWriter::remove(const Pomodoro& pomodoro)
{
    dbService.bind(removeQueryId,
                   ":uuid",
                   QVariant(QString::fromStdString(pomodoro.uuid())));
    dbService.executePrepared(removeQueryId);
}
