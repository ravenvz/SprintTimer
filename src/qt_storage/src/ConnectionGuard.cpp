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
#include "qt_storage/ConnectionGuard.h"
#include "qt_storage/DatabaseError.h"
#include <QSqlDatabase>

namespace sprint_timer::storage::qt_storage {

ConnectionGuard::ConnectionGuard(const QString& dbName_,
                                 const QString& connectionName_)
    : connectionName{connectionName_}
{
    auto db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    if (dbName_ == "file::memory:?cache=shared") {
        db.setConnectOptions("QSQLITE_OPEN_URI");
    }
    db.setDatabaseName(dbName_);
    if (!db.open() || db.isOpenError()) {
        throw DatabaseError{"Unable to open connection to the database", db};
    }
}

ConnectionGuard::~ConnectionGuard()
{
    QSqlDatabase::database(connectionName).close();
    QSqlDatabase::removeDatabase(connectionName);
}

} // namespace sprint_timer::storage::qt_storage
