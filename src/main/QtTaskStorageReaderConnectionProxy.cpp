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
#include "QtTaskStorageReaderConnectionProxy.h"
#include "qt_storage/QtTaskStorageReader.h"
#include <optional>

namespace {

thread_local std::optional<
    sprint_timer::storage::qt_storage::QtTaskStorageReader>
    localStorage;

} // namespace

namespace sprint_timer::compose {

QtTaskStorageReaderConnectionProxy::QtTaskStorageReaderConnectionProxy(
    ThreadConnectionHelper& connectionHelper_)
    : connectionHelper{connectionHelper_}
{
}

std::vector<entities::Task>
QtTaskStorageReaderConnectionProxy::unfinishedTasks()
{
    checkStorageInteractorInitialized();
    return localStorage->unfinishedTasks();
}

std::vector<entities::Task> QtTaskStorageReaderConnectionProxy::finishedTasks(
    const dw::DateRange& dateRange)
{
    checkStorageInteractorInitialized();
    return localStorage->finishedTasks(dateRange);
}

std::vector<entities::Task>
QtTaskStorageReaderConnectionProxy::allTasks(const dw::DateRange& dateRange)
{
    checkStorageInteractorInitialized();
    return localStorage->allTasks(dateRange);
}

std::vector<std::string> QtTaskStorageReaderConnectionProxy::allTags()
{
    checkStorageInteractorInitialized();
    return localStorage->allTags();
}

std::vector<entities::Task>
QtTaskStorageReaderConnectionProxy::findByUuid(const std::string& uuid)
{
    checkStorageInteractorInitialized();
    return localStorage.value().findByUuid(uuid);
}

void QtTaskStorageReaderConnectionProxy::checkStorageInteractorInitialized()
{
    if (!localStorage) {
        localStorage.emplace(connectionHelper.connectionName());
    }
}

} // namespace sprint_timer::compose
