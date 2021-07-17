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
#include "ThreadConnectionHelper.h"
#include "qt_storage/WorkerConnection.h"
#include <QString>
#include <optional>
#include <sstream>
#include <utility>
#include <thread>

#include <iostream>

namespace {

thread_local std::optional<sprint_timer::storage::qt_storage::WorkerConnection>
    connection;

thread_local std::optional<QString> name;

} // namespace

namespace sprint_timer::compose {

ThreadConnectionHelper::ThreadConnectionHelper(std::string sqliteFileName_)
    : sqliteFileName{std::move(sqliteFileName_)}
{
}

QString ThreadConnectionHelper::connectionName()
{
    if (!connection) {
        std::stringstream ss;
        ss << "conn_" << std::this_thread::get_id();
        name.emplace(QString::fromStdString(ss.str()));
        connection.emplace(QString::fromStdString(sqliteFileName), *name);
        std::cout << "Connection created: " << ss.str() << std::endl;
    }
    return connection.value().connectionName();
}

} // namespace sprint_timer::compose
