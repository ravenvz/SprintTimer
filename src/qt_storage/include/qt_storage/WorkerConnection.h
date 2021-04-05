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
#ifndef DB_HELPER_H
#define DB_HELPER_H

#include "qt_storage/ConnectionGuard.h"
#include "qt_storage/QueryError.h"
#include <QDebug>
#include <QHash>
#include <QObject>
#include <QSqlDatabase>
#include <QThread>
#include <QtSql>
#include <algorithm>
#include <memory>
#include <vector>

namespace sprint_timer::storage::qt_storage {

class WorkerConnection {
public:
    WorkerConnection(const QString& filename);

    WorkerConnection(const std::string& filename);

    ~WorkerConnection() = default;

    WorkerConnection(WorkerConnection&&) = delete;
    WorkerConnection& operator=(WorkerConnection&&) = delete;

    WorkerConnection(const WorkerConnection&) = delete;
    WorkerConnection& operator=(const WorkerConnection&) = delete;

    QString connectionName() const;

private:
    const QString filename;
    const QString connName{"SyncConnection"};
    qt_storage::ConnectionGuard connection{filename, connName};

    void setPragmas();
};

} // namespace sprint_timer::storage::qt_storage

#endif // DB_HELPER_H
