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
#ifndef CONNECTIONGUARD_H_6GDKXRYB
#define CONNECTIONGUARD_H_6GDKXRYB

#include <QString>

namespace sprint_timer::storage::qt_storage_impl {

struct ConnectionGuard {
    ConnectionGuard(const QString& dbName, const QString& connectionName);

    ~ConnectionGuard();

    ConnectionGuard(ConnectionGuard&&) = delete;
    ConnectionGuard& operator=(ConnectionGuard&&) = delete;

    ConnectionGuard(const ConnectionGuard&) = delete;
    ConnectionGuard& operator=(const ConnectionGuard&) = delete;

private:
    const QString connectionName;
};


} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: CONNECTIONGUARD_H_6GDKXRYB */
