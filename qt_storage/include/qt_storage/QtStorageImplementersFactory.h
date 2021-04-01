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

#include <QString>
#include <core/IStorageImplementersFactory.h>

namespace sprint_timer::storage::qt_storage {

class QtStorageImplementersFactory : public StorageImplementersFactory {
public:
    explicit QtStorageImplementersFactory(const QString& connectionName);

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
    QString connectionName;
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QT_STORAGE_IMPLEMENTERS_FACTORY_H_57Q0AHPC */
