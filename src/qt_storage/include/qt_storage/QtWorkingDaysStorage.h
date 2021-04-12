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
#ifndef QTWORKINGDAYSSTORAGE_H_EXLIBP9M
#define QTWORKINGDAYSSTORAGE_H_EXLIBP9M

#include <QSqlQuery>
#include "core/WorkScheduleStorage.h"

namespace sprint_timer::storage::qt_storage {

class QtWorkScheduleStorage : public WorkScheduleStorage {
public:
    explicit QtWorkScheduleStorage(const QString& connectionName);
    ~QtWorkScheduleStorage() override = default;

    QtWorkScheduleStorage(QtWorkScheduleStorage&&) = delete;
    QtWorkScheduleStorage& operator=(QtWorkScheduleStorage&&) = delete;

    QtWorkScheduleStorage(const QtWorkScheduleStorage&) = delete;
    QtWorkScheduleStorage& operator=(const QtWorkScheduleStorage&) = delete;

    void updateSchedule(const WorkSchedule& workSchedule) final;

    WorkSchedule schedule() final;

private:
    const QString connectionName;
    QSqlQuery storeScheduleQuery;
    QSqlQuery storeExceptionalDaysQuery;
};

} // namespace sprint_timer::storage::qt_storage

#endif /* end of include guard: QTWORKINGDAYSSTORAGE_H_EXLIBP9M */
