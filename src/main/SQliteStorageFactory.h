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
#include "core/IConfig.h"
#include "core/IStorageImplementersFactory.h"

namespace sprint_timer::compose {

class SQliteStorageFactory : public StorageImplementersFactory {
public:
    SQliteStorageFactory(ThreadConnectionHelper& connectionHelper,
                         IConfig& applicationSettings);

    std::unique_ptr<SprintStorage> sprintStorage() const override;

    std::unique_ptr<TaskStorage> taskStorage() const override;

    std::unique_ptr<OperationalRangeReader>
    operationalRangeReader() const override;

    std::unique_ptr<SprintDistributionReader>
    dailyDistReader(size_t numDays) const override;

    std::unique_ptr<SprintDistributionReader>
    weeklyDistReader(dw::Weekday firstDayOfWeek) const override;

    std::unique_ptr<SprintDistributionReader>
    monthlyDistReader() const override;

    std::unique_ptr<WorkScheduleStorage> scheduleStorage() const override;

private:
    ThreadConnectionHelper& connectionHelper;
    IConfig& applicationSettings;
};

} // namespace sprint_timer::compose

#endif /* end of include guard: SQLITESTORAGEFACTORY_H_ONDMSA1X */
