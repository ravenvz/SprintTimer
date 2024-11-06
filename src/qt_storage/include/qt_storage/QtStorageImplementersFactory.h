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
#ifndef QT_STORAGE_IMPLEMENTERS_FACTORY_H_57Q0AHPC
#define QT_STORAGE_IMPLEMENTERS_FACTORY_H_57Q0AHPC

#include "api/StorageImplementersFactory.h"
#include <QString>
#include <filesystem>

namespace sprint_timer::storage::qt_storage {

class QtStorageImplementersFactory : public api::StorageImplementersFactory {
public:
    explicit QtStorageImplementersFactory(const QString& connectionName);

    [[nodiscard]] auto
    sprintStorage() const -> std::unique_ptr<api::SprintStorage>;

    [[nodiscard]] std::unique_ptr<api::TaskStorage> taskStorage() const;

    [[nodiscard]] auto
    operationalRangeReader(const api::DateTimeProvider& dateTimeProvider) const
        -> std::unique_ptr<api::OperationalRangeReader>;

    [[nodiscard]] auto dailyDistReader(size_t numDays) const
        -> std::unique_ptr<api::SprintDistributionReader>;

    [[nodiscard]] auto weeklyDistReader(dw::Weekday firstDayOfWeek) const
        -> std::unique_ptr<api::SprintDistributionReader>;

    [[nodiscard]] auto
    monthlyDistReader() const -> std::unique_ptr<api::SprintDistributionReader>;

    [[nodiscard]] auto
    scheduleStorage() const -> std::unique_ptr<api::WorkScheduleStorage>;

private:
    QString connectionName;
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QT_STORAGE_IMPLEMENTERS_FACTORY_H_57Q0AHPC */
