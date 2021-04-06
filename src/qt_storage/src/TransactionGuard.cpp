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
#include "qt_storage/TransactionGuard.h"
#include "qt_storage/DatabaseError.h"
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlDriver>

namespace sprint_timer::storage::qt_storage {

TransactionGuard::TransactionGuard(const QString& connectionName_)
    : connectionName{connectionName_}
{
    if (!QSqlDatabase::database(connectionName).driver()->hasFeature(QSqlDriver::Transactions))
        throw DatabaseError{"SQL driver does not support transactions",
                            QSqlDatabase::database(connectionName)};

    if (!QSqlDatabase::database(connectionName).transaction())
        throw DatabaseError{"Error starting transaction",
                            QSqlDatabase::database(connectionName)};
}

TransactionGuard::~TransactionGuard()
{
    if (!hasCommited) {
        if (!QSqlDatabase::database(connectionName).rollback())
            qDebug()
                << "Cannot rollback transaction "
                << QSqlDatabase::database(connectionName).lastError().text();
    }
}

void TransactionGuard::commit()
{
    if (!QSqlDatabase::database(connectionName).commit())
        throw DatabaseError{"Cannot commit transaction",
                            QSqlDatabase::database(connectionName)};
    hasCommited = true;
}

} // namespace sprint_timer::storage::qt_storage

