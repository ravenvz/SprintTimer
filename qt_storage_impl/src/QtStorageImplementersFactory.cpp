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
#include "qt_storage_impl/QtStorageImplementersFactory.h"
#include "qt_storage_impl/QtSprintDistributionReader.h"
#include "qt_storage_impl/QtSprintStorageReader.h"
#include "qt_storage_impl/QtSprintStorageWriter.h"
#include "qt_storage_impl/QtTaskStorageReader.h"
#include "qt_storage_impl/QtTaskStorageWriter.h"
#include "qt_storage_impl/QtWorkingDaysReader.h"
#include "qt_storage_impl/QtWorkingDaysWriter.h"
#include "qt_storage_impl/QtYearRangeReader.h"

namespace sprint_timer::storage::qt_storage_impl {

QtStorageImplementersFactory::QtStorageImplementersFactory(
    DBService& dbService, const IConfig& settings)
    : dbService{dbService}
    , settings{settings}
{
}

std::unique_ptr<ISprintStorageReader>
QtStorageImplementersFactory::createSprintStorageReader() const
{
    return std::make_unique<QtSprintStorageReader>(dbService);
}

std::unique_ptr<ISprintStorageWriter>
QtStorageImplementersFactory::createSprintStorageWriter() const
{
    return std::make_unique<QtSprintStorageWriter>(dbService);
}

std::unique_ptr<IYearRangeReader>
QtStorageImplementersFactory::createYearRangeReader() const
{
    return std::make_unique<QtYearRangeReader>(dbService);
}

std::unique_ptr<ISprintDistributionReader>
QtStorageImplementersFactory::createSprintDailyDistributionReader() const
{
    constexpr size_t numDays{30};
    return std::make_unique<QtSprintDailyDistributionReader>(dbService,
                                                             numDays);
}

std::unique_ptr<ISprintDistributionReader>
QtStorageImplementersFactory::createSprintWeeklyDistributionReader(
    FirstDayOfWeek firstDayOfWeek) const
{
    constexpr size_t numWeeks{12};
    if (firstDayOfWeek == FirstDayOfWeek::Monday)
        return std::make_unique<QtSprintDistReaderMondayFirst>(dbService,
                                                               numWeeks);
    return std::make_unique<QtSprintDistReaderSundayFirst>(dbService, numWeeks);
}

std::unique_ptr<ISprintDistributionReader>
QtStorageImplementersFactory::createSprintMonthlyDistributionReader() const
{
    constexpr size_t numMonths{12};
    return std::make_unique<QtSprintMonthlyDistributionReader>(dbService,
                                                               numMonths);
}

std::unique_ptr<ITaskStorageReader>
QtStorageImplementersFactory::createTaskStorageReader() const
{
    return std::make_unique<QtTaskStorageReader>(dbService);
}

std::unique_ptr<ITaskStorageWriter>
QtStorageImplementersFactory::createTaskStorageWriter() const
{
    return std::make_unique<QtTaskStorageWriter>(dbService);
}

std::unique_ptr<IWorkingDaysReader>
QtStorageImplementersFactory::createWorkingDaysReader() const
{
    return std::make_unique<QtWorkingDaysReader>(dbService, settings);
}

std::unique_ptr<IWorkingDaysWriter>
QtStorageImplementersFactory::createWorkingDaysWriter() const
{
    return std::make_unique<QtWorkingDaysWriter>(dbService);
}

} // namespace sprint_timer::storage::qt_storage_impl
