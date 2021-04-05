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
#include "qt_storage/WorkerConnection.h"
#include "qt_storage/DatabaseError.h"
#include "qt_storage/DatabaseInitializer.h"
#include "qt_storage/QueryError.h"
#include "qt_storage/utils/QueryUtils.h"
#include <iostream>

namespace sprint_timer::storage::qt_storage {

WorkerConnection::WorkerConnection(const QString& filename_)
    : filename{filename_}
{
    setPragmas();
}

WorkerConnection::WorkerConnection(const std::string& filename_)
    : filename{QString::fromStdString(filename_)}
{
}

QString WorkerConnection::connectionName() const { return connName; }

void WorkerConnection::setPragmas()
{
    QSqlQuery query{QSqlDatabase::database(connName)};
    tryExecute(query, "PRAGMA foreign_keys = ON");
}

} // namespace sprint_timer::storage::qt_storage

