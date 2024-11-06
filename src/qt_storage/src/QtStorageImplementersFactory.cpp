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
#include "qt_storage/QtStorageImplementersFactory.h"
#include "qt_storage/QtOperationalRangeReader.h"
#include "qt_storage/QtSprintDistributionReader.h"
#include "qt_storage/QtSprintStorage.h"
#include "qt_storage/QtTaskStorage.h"
#include "qt_storage/QtWorkScheduleStorage.h"
#include <fstream>
#include <utility>

namespace sprint_timer::storage::qt_storage {

QtStorageImplementersFactory::QtStorageImplementersFactory(
    const QString& connectionName_)
    : connectionName{connectionName_}
{
}

auto QtStorageImplementersFactory::sprintStorage() const
    -> std::unique_ptr<api::SprintStorage>
{
    return std::make_unique<QtSprintStorage>(
        std::make_unique<QtSprintStorageReader>(connectionName),
        std::make_unique<QtSprintStorageWriter>(connectionName));
}

auto QtStorageImplementersFactory::taskStorage() const
    -> std::unique_ptr<api::TaskStorage>
{
    return std::make_unique<QtTaskStorage>(
        std::make_unique<QtTaskStorageReader>(connectionName),
        std::make_unique<QtTaskStorageWriter>(connectionName));
}

auto QtStorageImplementersFactory::operationalRangeReader(
    const api::DateTimeProvider& dateTimeProvider) const
    -> std::unique_ptr<api::OperationalRangeReader>
{
    return std::make_unique<QtOperationalRangeReader>(connectionName,
                                                      dateTimeProvider);
}

auto QtStorageImplementersFactory::dailyDistReader(size_t numDays) const
    -> std::unique_ptr<api::SprintDistributionReader>
{
    return std::make_unique<QtSprintDailyDistributionReader>(connectionName,
                                                             numDays);
}

auto QtStorageImplementersFactory::weeklyDistReader(dw::Weekday firstDayOfWeek)
    const -> std::unique_ptr<api::SprintDistributionReader>
{
    constexpr size_t numWeeks{12};
    if (firstDayOfWeek == dw::Weekday::Monday)
        return std::make_unique<QtSprintDistReaderMondayFirst>(connectionName,
                                                               numWeeks);
    return std::make_unique<QtSprintDistReaderSundayFirst>(connectionName,
                                                           numWeeks);
}

auto QtStorageImplementersFactory::monthlyDistReader() const
    -> std::unique_ptr<api::SprintDistributionReader>
{
    constexpr size_t numMonths{12};
    return std::make_unique<QtSprintMonthlyDistributionReader>(connectionName,
                                                               numMonths);
}

auto QtStorageImplementersFactory::scheduleStorage() const
    -> std::unique_ptr<api::WorkScheduleStorage>
{
    return std::make_unique<QtWorkScheduleStorage>(connectionName);
}

} // namespace sprint_timer::storage::qt_storage
