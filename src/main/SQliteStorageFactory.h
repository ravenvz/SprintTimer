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
#ifndef SQLITESTORAGEFACTORY_H_ONDMSA1X
#define SQLITESTORAGEFACTORY_H_ONDMSA1X

#include "ThreadConnectionHelper.h"
#include "api/DateTimeProvider.h"
#include "api/IConfig.h"
#include "api/StorageImplementersFactory.h"
#include <filesystem>

namespace sprint_timer::compose {

class SQliteStorageFactory : public api::StorageImplementersFactory {
public:
    SQliteStorageFactory(ThreadConnectionHelper& connectionHelper,
                         api::IConfig& applicationSettings);

    [[nodiscard]] auto
    sprintStorage() const -> std::unique_ptr<api::SprintStorage> override;

    [[nodiscard]] auto
    taskStorage() const -> std::unique_ptr<api::TaskStorage> override;

    [[nodiscard]] auto
    operationalRangeReader(const api::DateTimeProvider& dateTimeProvider) const
        -> std::unique_ptr<api::OperationalRangeReader> override;

    [[nodiscard]] auto dailyDistReader(size_t numDays) const
        -> std::unique_ptr<api::SprintDistributionReader> override;

    [[nodiscard]] auto weeklyDistReader(dw::Weekday firstDayOfWeek) const
        -> std::unique_ptr<api::SprintDistributionReader> override;

    [[nodiscard]] auto monthlyDistReader() const
        -> std::unique_ptr<api::SprintDistributionReader> override;

    [[nodiscard]] auto scheduleStorage() const
        -> std::unique_ptr<api::WorkScheduleStorage> override;

private:
    ThreadConnectionHelper& connectionHelper;
    api::IConfig& applicationSettings;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: SQLITESTORAGEFACTORY_H_ONDMSA1X */
