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
#ifndef WORKERCONNECTION_H_QYWK9G25
#define WORKERCONNECTION_H_QYWK9G25

#include "qt_storage/ConnectionGuard.h"
#include "qt_storage/QueryError.h"
#include <QString>

namespace sprint_timer::storage::qt_storage {

class WorkerConnection {
public:
    WorkerConnection(QString filename, QString connectionName);

    ~WorkerConnection() = default;

    WorkerConnection(WorkerConnection&&) = delete;
    WorkerConnection& operator=(WorkerConnection&&) = delete;

    WorkerConnection(const WorkerConnection&) = delete;
    WorkerConnection& operator=(const WorkerConnection&) = delete;

    QString connectionName() const;

private:
    QString filename;
    QString connName;
    qt_storage::ConnectionGuard connection;

    void setPragmas();
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: WORKERCONNECTION_H_QYWK9G25 */

