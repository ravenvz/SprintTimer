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
#include "QtWorkScheduleStorageConnectionProxy.h"
#include "qt_storage/QtWorkScheduleStorage.h"
#include "qt_storage/WorkerConnection.h"
#include <optional>
#include <utility>

namespace {

thread_local std::optional<
    sprint_timer::storage::qt_storage::QtWorkScheduleStorage>
    localStorage;

} // namespace

namespace sprint_timer::compose {

QtWorkScheduleStorageConnectionProxy::QtWorkScheduleStorageConnectionProxy(
    ThreadConnectionHelper& connectionHelper_)
    : connectionHelper{connectionHelper_}
{
}

void QtWorkScheduleStorageConnectionProxy::updateSchedule(
    const WorkSchedule& workSchedule)
{
    if (!localStorage) {
        createStorageInteractor();
    }
    localStorage->updateSchedule(workSchedule);
}

WorkSchedule QtWorkScheduleStorageConnectionProxy::schedule()
{
    if (!localStorage) {
        createStorageInteractor();
    }
    return localStorage->schedule();
}

void QtWorkScheduleStorageConnectionProxy::createStorageInteractor()
{
    localStorage.emplace(connectionHelper.connectionName());
}

} // namespace sprint_timer::compose
