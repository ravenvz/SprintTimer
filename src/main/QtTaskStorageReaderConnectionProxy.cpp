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

auto QtTaskStorageReaderConnectionProxy::unfinishedTasks() -> std::vector<Task>
{
    checkStorageInteractorInitialized();
    return localStorage->unfinishedTasks();
}

auto QtTaskStorageReaderConnectionProxy::finishedTasks(
    const dw::DateRange& dateRange) -> std::vector<Task>
{
    checkStorageInteractorInitialized();
    return localStorage->finishedTasks(dateRange);
}

auto QtTaskStorageReaderConnectionProxy::allTasks(
    const dw::DateRange& dateRange) -> std::vector<Task>
{
    checkStorageInteractorInitialized();
    return localStorage->allTasks(dateRange);
}

auto QtTaskStorageReaderConnectionProxy::allTags() -> std::vector<std::string>
{
    checkStorageInteractorInitialized();
    return localStorage->allTags();
}

auto QtTaskStorageReaderConnectionProxy::findByUuid(const std::string& uuid)
    -> std::vector<Task>
{
    checkStorageInteractorInitialized();
    return localStorage.value().findByUuid(uuid);
}

auto QtTaskStorageReaderConnectionProxy::findMatching(
    std::span<const std::string> uuids) -> std::vector<Task>
{
    checkStorageInteractorInitialized();
    return localStorage.value().findMatching(uuids);
}

auto QtTaskStorageReaderConnectionProxy::taskTree() -> TaskTree
{
    checkStorageInteractorInitialized();
    return localStorage.value().taskTree();
}

auto QtTaskStorageReaderConnectionProxy::checkStorageInteractorInitialized()
    -> void
{
    if (!localStorage) {
        localStorage.emplace(connectionHelper.connectionName());
    }
}

} // namespace sprint_timer::compose
