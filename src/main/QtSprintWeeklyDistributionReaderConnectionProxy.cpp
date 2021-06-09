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
#include "QtSprintWeeklyDistributionReaderConnectionProxy.h"
#include "qt_storage/QtSprintDistributionReader.h"
#include <optional>

namespace {

thread_local std::optional<
    sprint_timer::storage::qt_storage::QtSprintDistReaderMondayFirst>
    mondayFirstReader;

thread_local std::optional<
    sprint_timer::storage::qt_storage::QtSprintDistReaderSundayFirst>
    sundayFirstReader;

} // namespace

namespace sprint_timer::compose {

QtSprintWeeklyDistributionReaderConnectionProxy::
    QtSprintWeeklyDistributionReaderConnectionProxy(
        ThreadConnectionHelper& connectionHelper_,
        size_t numWeeks_,
        IConfig& applicationSettings_)
    : connectionHelper{connectionHelper_}
    , numWeeks{numWeeks_}
    , applicationSettings{applicationSettings_}
{
}

std::vector<int>
QtSprintWeeklyDistributionReaderConnectionProxy::sprintDistribution(
    const dw::DateRange& dateRange)
{
    checkStorageInteractorInitialized();
    switch (applicationSettings.firstDayOfWeek()) {
    case dw::Weekday::Monday:
        return mondayFirstReader->sprintDistribution(dateRange);
        break;
    default:
        return sundayFirstReader->sprintDistribution(dateRange);
    }
}

void QtSprintWeeklyDistributionReaderConnectionProxy::
    checkStorageInteractorInitialized()
{
    if (!mondayFirstReader) {
        mondayFirstReader.emplace(connectionHelper.connectionName(), numWeeks);
    }
    if (!sundayFirstReader) {
        sundayFirstReader.emplace(connectionHelper.connectionName(), numWeeks);
    }
}

} // namespace sprint_timer::compose
