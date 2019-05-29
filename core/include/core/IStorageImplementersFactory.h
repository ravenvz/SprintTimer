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
#include "core/IOperationalRangeReader.h"
#include "core/ISprintDistributionReader.h"
#include "core/ISprintStorage.h"
#include "core/ITaskStorageReader.h"
#include "core/ITaskStorageWriter.h"
#include "core/IWorkingDaysStorage.h"
#include <memory>

namespace sprint_timer {

class IStorageImplementersFactory {
public:
    virtual ~IStorageImplementersFactory() = default;

    virtual std::unique_ptr<ISprintStorage> createSprintStorage() const = 0;

    virtual std::unique_ptr<IOperationalRangeReader>
    createOperationalRangeReader() const = 0;

    virtual std::unique_ptr<ISprintDistributionReader>
    createSprintDailyDistributionReader() const = 0;

    virtual std::unique_ptr<ISprintDistributionReader>
    createSprintWeeklyDistributionReader(dw::Weekday firstDayOfWeek) const = 0;

    virtual std::unique_ptr<ISprintDistributionReader>
    createSprintMonthlyDistributionReader() const = 0;

    virtual std::unique_ptr<ITaskStorageReader>
    createTaskStorageReader() const = 0;

    virtual std::unique_ptr<ITaskStorageWriter>
    createTaskStorageWriter() const = 0;

    virtual std::unique_ptr<IWorkingDaysStorage>
    createWorkingDaysStorage() const = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: ISTORAGEIMPLEMENTERSFACTORY_H_PRIUAOH6 */
