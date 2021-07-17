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
#include "SQliteStorageFactory.h"
#include "QtOperationalRangeReaderConnectionProxy.h"
#include "QtSprintDailyDistributionReaderConnectionProxy.h"
#include "QtSprintStorageReaderConnectionProxy.h"
#include "QtSprintWeeklyDistributionReaderConnectionProxy.h"
#include "QtSprintMonthlyDistributionReaderConnectionProxy.h"
#include "QtTaskStorageReaderConnectionProxy.h"
#include "QtWorkScheduleStorageConnectionProxy.h"
#include "qt_storage/QtOperationalRangeReader.h"
#include "qt_storage/QtSprintDistributionReader.h"
#include "qt_storage/QtSprintStorage.h"
#include "qt_storage/QtStorageImplementersFactory.h"
#include "qt_storage/QtTaskStorage.h"
#include "qt_storage/QtWorkScheduleStorage.h"
#include <optional>

using namespace sprint_timer::storage::qt_storage;

namespace sprint_timer::compose {

SQliteStorageFactory::SQliteStorageFactory(
    ThreadConnectionHelper& connectionHelper_, IConfig& applicationSettings_)
    : connectionHelper{connectionHelper_}
    , applicationSettings{applicationSettings_}
{
}

std::unique_ptr<SprintStorage> SQliteStorageFactory::sprintStorage() const
{
    return std::make_unique<QtSprintStorage>(
        std::make_unique<QtSprintStorageReaderConnectionProxy>(
            connectionHelper),
        std::make_unique<QtSprintStorageWriter>(
            connectionHelper.connectionName()));
}

std::unique_ptr<TaskStorage> SQliteStorageFactory::taskStorage() const
{
    return std::make_unique<QtTaskStorage>(
        std::make_unique<QtTaskStorageReaderConnectionProxy>(connectionHelper),
        std::make_unique<QtTaskStorageWriter>(
            connectionHelper.connectionName()));
}

std::unique_ptr<OperationalRangeReader>
SQliteStorageFactory::operationalRangeReader() const
{
    return std::make_unique<QtOperationalRangeReaderConnectionProxy>(
        connectionHelper);
}

std::unique_ptr<SprintDistributionReader>
SQliteStorageFactory::dailyDistReader(size_t numDays) const
{
    return std::make_unique<QtSprintDailyDistributionReaderConnectionProxy>(
        connectionHelper, numDays);
}

std::unique_ptr<SprintDistributionReader>
    SQliteStorageFactory::weeklyDistReader(dw::Weekday /*firstDayOfWeek*/) const
{
    constexpr size_t numWeeks{12};
    return std::make_unique<QtSprintWeeklyDistributionReaderConnectionProxy>(
        connectionHelper, numWeeks, applicationSettings);
}

std::unique_ptr<SprintDistributionReader>
SQliteStorageFactory::monthlyDistReader() const
{
    constexpr size_t numMonths{12};
    return std::make_unique<QtSprintMonthlyDistributionReaderConnectionProxy>(
        connectionHelper, numMonths);
}

std::unique_ptr<WorkScheduleStorage>
SQliteStorageFactory::scheduleStorage() const
{
    return std::make_unique<QtWorkScheduleStorageConnectionProxy>(
        connectionHelper);
}
} // namespace sprint_timer::compose
