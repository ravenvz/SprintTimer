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
#include "qt_storage/WorkerConnection.h"
#include "qt_storage/DatabaseError.h"
#include "qt_storage/DatabaseInitializer.h"
#include "qt_storage/QueryError.h"
#include "qt_storage/utils/QueryUtils.h"
#include <utility>

#include <QDebug>

namespace sprint_timer::storage::qt_storage {

WorkerConnection::WorkerConnection(QString filename_, QString connectionName_)
    : filename{std::move(filename_)}
    , connName{std::move(connectionName_)}
    , connection{filename, connName}
{
    qDebug() << "Worker connection: " << connName;
    setPragmas();
}

QString WorkerConnection::connectionName() const { return connName; }

void WorkerConnection::setPragmas()
{
    QSqlQuery query{QSqlDatabase::database(connName)};
    tryExecute(query, "PRAGMA foreign_keys = ON");
}

} // namespace sprint_timer::storage::qt_storage

