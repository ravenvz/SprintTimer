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
#ifndef ISTORAGEIMPLEMENTERSFACTORY_H_PRIUAOH6
#define ISTORAGEIMPLEMENTERSFACTORY_H_PRIUAOH6

#include "api/DateTimeProvider.h"
#include "api/OperationalRangeReader.h"
#include "api/SprintDistributionReader.h"
#include "api/SprintStorage.h"
#include "api/TaskStorage.h"
#include "api/TaskTreeMetadataStorage.h"
#include "api/WorkScheduleStorage.h"
#include <memory>

namespace sprint_timer::api {

class StorageImplementersFactory {
public:
    virtual ~StorageImplementersFactory() = default;

    [[nodiscard]] virtual auto
    sprintStorage() const -> std::unique_ptr<SprintStorage> = 0;

    [[nodiscard]] virtual auto
    taskStorage() const -> std::unique_ptr<TaskStorage> = 0;

    [[nodiscard]] virtual auto
    operationalRangeReader(const api::DateTimeProvider& dateTimeProvider) const
        -> std::unique_ptr<OperationalRangeReader> = 0;

    [[nodiscard]] virtual auto dailyDistReader(size_t numDays) const
        -> std::unique_ptr<SprintDistributionReader> = 0;

    [[nodiscard]] virtual auto weeklyDistReader(dw::Weekday firstDayOfWeek)
        const -> std::unique_ptr<SprintDistributionReader> = 0;

    [[nodiscard]] virtual auto
    monthlyDistReader() const -> std::unique_ptr<SprintDistributionReader> = 0;

    [[nodiscard]] virtual auto
    scheduleStorage() const -> std::unique_ptr<WorkScheduleStorage> = 0;
};

} // namespace sprint_timer::api

#endif /* end of include guard: ISTORAGEIMPLEMENTERSFACTORY_H_PRIUAOH6 */
