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
#include "QtSprintMonthlyDistributionReaderConnectionProxy.h"
#include "QtSprintStorageReaderConnectionProxy.h"
#include "QtSprintWeeklyDistributionReaderConnectionProxy.h"
#include "QtTaskStorageReaderConnectionProxy.h"
#include "QtWorkScheduleStorageConnectionProxy.h"
#include "qt_storage/QtOperationalRangeReader.h"
#include "qt_storage/QtSprintDistributionReader.h"
#include "qt_storage/QtSprintStorage.h"
#include "qt_storage/QtStorageImplementersFactory.h"
#include "qt_storage/QtTaskStorage.h"
#include "qt_storage/QtWorkScheduleStorage.h"
#include <optional>
#include <utility>

using namespace sprint_timer::storage::qt_storage;

namespace {

constexpr std::string_view taskTreeFileName{"taskTree"};

} // namespace

namespace sprint_timer::compose {

SQliteStorageFactory::SQliteStorageFactory(
    ThreadConnectionHelper& connectionHelper_,
    api::IConfig& applicationSettings_)
    : connectionHelper{connectionHelper_}
    , applicationSettings{applicationSettings_}
{
}

auto SQliteStorageFactory::sprintStorage() const
    -> std::unique_ptr<api::SprintStorage>
{
    return std::make_unique<QtSprintStorage>(
        std::make_unique<QtSprintStorageReaderConnectionProxy>(
            connectionHelper),
        std::make_unique<QtSprintStorageWriter>(
            connectionHelper.connectionName()));
}

auto SQliteStorageFactory::taskStorage() const
    -> std::unique_ptr<api::TaskStorage>
{
    return std::make_unique<QtTaskStorage>(
        std::make_unique<QtTaskStorageReaderConnectionProxy>(connectionHelper),
        std::make_unique<QtTaskStorageWriter>(
            connectionHelper.connectionName()));
}

auto SQliteStorageFactory::operationalRangeReader(
    const api::DateTimeProvider& dateTimeProvider) const
    -> std::unique_ptr<api::OperationalRangeReader>
{
    return std::make_unique<QtOperationalRangeReaderConnectionProxy>(
        connectionHelper, dateTimeProvider);
}

auto SQliteStorageFactory::dailyDistReader(size_t numDays) const
    -> std::unique_ptr<api::SprintDistributionReader>
{
    return std::make_unique<QtSprintDailyDistributionReaderConnectionProxy>(
        connectionHelper, numDays);
}

auto SQliteStorageFactory::weeklyDistReader(dw::Weekday /*firstDayOfWeek*/)
    const -> std::unique_ptr<api::SprintDistributionReader>
{
    constexpr size_t numWeeks{12};
    return std::make_unique<QtSprintWeeklyDistributionReaderConnectionProxy>(
        connectionHelper, numWeeks, applicationSettings);
}

auto SQliteStorageFactory::monthlyDistReader() const
    -> std::unique_ptr<api::SprintDistributionReader>
{
    constexpr size_t numMonths{12};
    return std::make_unique<QtSprintMonthlyDistributionReaderConnectionProxy>(
        connectionHelper, numMonths);
}

auto SQliteStorageFactory::scheduleStorage() const
    -> std::unique_ptr<api::WorkScheduleStorage>
{
    return std::make_unique<QtWorkScheduleStorageConnectionProxy>(
        connectionHelper);
}

} // namespace sprint_timer::compose
