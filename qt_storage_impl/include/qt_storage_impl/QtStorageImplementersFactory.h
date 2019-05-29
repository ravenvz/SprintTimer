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
#ifndef QT_STORAGE_IMPLEMENTERS_FACTORY_H_57Q0AHPC
#define QT_STORAGE_IMPLEMENTERS_FACTORY_H_57Q0AHPC

#include "qt_storage_impl/DBService.h"
#include <core/IStorageImplementersFactory.h>

namespace sprint_timer::storage::qt_storage_impl {

class QtStorageImplementersFactory : public IStorageImplementersFactory {
public:
    explicit QtStorageImplementersFactory(DBService& dbService);

    std::unique_ptr<ISprintStorage> createSprintStorage() const override;

    std::unique_ptr<IOperationalRangeReader>
    createOperationalRangeReader() const override;

    std::unique_ptr<ISprintDistributionReader>
    createSprintDailyDistributionReader() const override;

    std::unique_ptr<ISprintDistributionReader>
    createSprintWeeklyDistributionReader(
        dw::Weekday firstDayOfWeek) const override;

    std::unique_ptr<ISprintDistributionReader>
    createSprintMonthlyDistributionReader() const override;

    std::unique_ptr<ITaskStorage> createTaskStorage() const override;

    std::unique_ptr<IWorkingDaysStorage>
    createWorkingDaysStorage() const override;

private:
    DBService& dbService;
};

} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: QT_STORAGE_IMPLEMENTERS_FACTORY_H_57Q0AHPC */
