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
#include "QtSprintStorageReaderConnectionProxy.h"
#include "qt_storage/QtSprintStorageReader.h"
#include "qt_storage/WorkerConnection.h"
#include <optional>
#include <utility>

namespace {

thread_local std::optional<
    sprint_timer::storage::qt_storage::QtSprintStorageReader>
    localStorage;

} // namespace

namespace sprint_timer::compose {

QtSprintStorageReaderConnectionProxy::QtSprintStorageReaderConnectionProxy(
    ThreadConnectionHelper& connectionHelper_)
    : connectionHelper{connectionHelper_}
{
}

std::vector<entities::Sprint>
QtSprintStorageReaderConnectionProxy::findByDateRange(
    const dw::DateRange& dateRange)
{
    if (!localStorage) {
        createStorageInteractor();
    }
    return localStorage->findByDateRange(dateRange);
}

std::vector<entities::Sprint>
QtSprintStorageReaderConnectionProxy::findByTaskUuid(
    const std::string& taskUuid)
{
    if (!localStorage) {
        createStorageInteractor();
    }
    return localStorage->findByTaskUuid(taskUuid);
}

std::vector<entities::Sprint>
QtSprintStorageReaderConnectionProxy::findByUuid(const std::string& uuid)
{
    if (!localStorage) {
        createStorageInteractor();
    }
    return localStorage->findByUuid(uuid);
}

void QtSprintStorageReaderConnectionProxy::createStorageInteractor()
{
    localStorage.emplace(connectionHelper.connectionName());
}

} // namespace sprint_timer::compose
