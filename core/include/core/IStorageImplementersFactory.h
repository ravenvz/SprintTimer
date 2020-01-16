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
#ifndef ISTORAGEIMPLEMENTERSFACTORY_H_PRIUAOH6
#define ISTORAGEIMPLEMENTERSFACTORY_H_PRIUAOH6

#include "core/IConfig.h"
#include "core/OperationalRangeReader.h"
#include "core/SprintDistributionReader.h"
#include "core/SprintStorage.h"
#include "core/TaskStorage.h"
#include "core/WorkScheduleStorage.h"
#include <memory>

namespace sprint_timer {

class IStorageImplementersFactory {
public:
    virtual ~IStorageImplementersFactory() = default;

    virtual std::unique_ptr<SprintStorage> createSprintStorage() const = 0;

    virtual std::unique_ptr<OperationalRangeReader>
    createOperationalRangeReader() const = 0;

    virtual std::unique_ptr<SprintDistributionReader>
    createSprintDailyDistributionReader() const = 0;

    virtual std::unique_ptr<SprintDistributionReader>
    createSprintWeeklyDistributionReader(dw::Weekday firstDayOfWeek) const = 0;

    virtual std::unique_ptr<SprintDistributionReader>
    createSprintMonthlyDistributionReader() const = 0;

    virtual std::unique_ptr<TaskStorage> createTaskStorage() const = 0;

    virtual std::unique_ptr<WorkScheduleStorage>
    createWorkingDaysStorage() const = 0;
};

class StorageImplementersFactory {
public:
    virtual ~StorageImplementersFactory() = default;

    virtual std::unique_ptr<SprintStorage> sprintStorage() const = 0;

    virtual std::unique_ptr<TaskStorage> taskStorage() const = 0;

    virtual std::unique_ptr<OperationalRangeReader>
    operationalRangeReader() const = 0;

    virtual std::unique_ptr<SprintDistributionReader>
    dailyDistReader() const = 0;

    virtual std::unique_ptr<SprintDistributionReader>
    weeklyDistReader(dw::Weekday firstDayOfWeek) const = 0;

    virtual std::unique_ptr<SprintDistributionReader>
    monthlyDistReader() const = 0;

    virtual std::unique_ptr<WorkScheduleStorage> scheduleStorage() const = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: ISTORAGEIMPLEMENTERSFACTORY_H_PRIUAOH6 */
