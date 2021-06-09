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
#include "QtSprintMonthlyDistributionReaderConnectionProxy.h"
#include "qt_storage/QtSprintDistributionReader.h"
#include <optional>

namespace {

std::optional<
    sprint_timer::storage::qt_storage::QtSprintMonthlyDistributionReader>
    localStorage;

} // namespace

namespace sprint_timer::compose {

QtSprintMonthlyDistributionReaderConnectionProxy::
    QtSprintMonthlyDistributionReaderConnectionProxy(
        ThreadConnectionHelper& connectionHelper_, size_t numMonths_)
    : connectionHelper{connectionHelper_}
    , numMonths{numMonths_}
{
}

std::vector<int>
QtSprintMonthlyDistributionReaderConnectionProxy::sprintDistribution(
    const dw::DateRange& dateRange)
{
    checkStorageInteractorInitialized();
    return localStorage->sprintDistribution(dateRange);
}

void QtSprintMonthlyDistributionReaderConnectionProxy::
    checkStorageInteractorInitialized()
{
    if (!localStorage) {
        localStorage.emplace(connectionHelper.connectionName(), numMonths);
    }
}

} // namespace sprint_timer::compose
