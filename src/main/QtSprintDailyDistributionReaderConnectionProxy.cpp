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
#include "QtSprintDailyDistributionReaderConnectionProxy.h"
#include "qt_storage/QtSprintDistributionReader.h"
#include <optional>

namespace {

thread_local std::optional<
    sprint_timer::storage::qt_storage::QtSprintDailyDistributionReader>
    localStorage{std::nullopt};

} // namespace

namespace sprint_timer::compose {

QtSprintDailyDistributionReaderConnectionProxy::
    QtSprintDailyDistributionReaderConnectionProxy(
        ThreadConnectionHelper& connectionHelper_, size_t numDays_)
    : connectionHelper{connectionHelper_}
    , numDays{numDays_}
{
}

std::vector<int>
QtSprintDailyDistributionReaderConnectionProxy::sprintDistribution(
    const dw::DateRange& dateRange)
{
    if (!localStorage) {
        createStorageInteractor();
    }
    return localStorage->sprintDistribution(dateRange);
}

void QtSprintDailyDistributionReaderConnectionProxy::createStorageInteractor()
{
    localStorage.emplace(connectionHelper.connectionName(), numDays);
}

} // namespace sprint_timer::compose
