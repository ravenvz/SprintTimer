/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "qt_storage/QtWorkingDaysStorage.h"

namespace sprint_timer::storage::qt_storage {

QtStorageImplementersFactory::QtStorageImplementersFactory(
    const QString& connectionName_)
    : connectionName{connectionName_}
{
}

std::unique_ptr<SprintStorage>
QtStorageImplementersFactory::sprintStorage() const
{
    return std::make_unique<QtSprintStorage>(
        std::make_unique<QtSprintStorageReader>(connectionName),
        std::make_unique<QtSprintStorageWriter>(connectionName));
}

std::unique_ptr<TaskStorage> QtStorageImplementersFactory::taskStorage() const
{
    return std::make_unique<QtTaskStorage>(
        std::make_unique<QtTaskStorageReader>(connectionName),
        std::make_unique<QtTaskStorageWriter>(connectionName));
}

std::unique_ptr<OperationalRangeReader>
QtStorageImplementersFactory::operationalRangeReader() const
{
    return std::make_unique<QtOperationalRangeReader>(connectionName);
}

std::unique_ptr<SprintDistributionReader>
QtStorageImplementersFactory::dailyDistReader() const
{
    constexpr size_t numDays{30};
    return std::make_unique<QtSprintDailyDistributionReader>(connectionName,
                                                             numDays);
}

std::unique_ptr<SprintDistributionReader>
QtStorageImplementersFactory::weeklyDistReader(dw::Weekday firstDayOfWeek) const
{
    constexpr size_t numWeeks{12};
    if (firstDayOfWeek == dw::Weekday::Monday)
        return std::make_unique<QtSprintDistReaderMondayFirst>(connectionName,
                                                               numWeeks);
    return std::make_unique<QtSprintDistReaderSundayFirst>(connectionName,
                                                           numWeeks);
}

std::unique_ptr<SprintDistributionReader>
QtStorageImplementersFactory::monthlyDistReader() const
{
    constexpr size_t numMonths{12};
    return std::make_unique<QtSprintMonthlyDistributionReader>(connectionName,
                                                               numMonths);
}

std::unique_ptr<WorkScheduleStorage>
QtStorageImplementersFactory::scheduleStorage() const
{
    return std::make_unique<QtWorkScheduleStorage>(connectionName);
}

} // namespace sprint_timer::storage::qt_storage
