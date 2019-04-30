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
#ifndef QTWORKINGDAYSWRITER_H_GMTRHCHZ
#define QTWORKINGDAYSWRITER_H_GMTRHCHZ

#include "qt_storage_impl/DBService.h"
#include <core/IWorkingDaysWriter.h>

namespace sprint_timer::storage::qt_storage_impl {

class QtWorkingDaysWriter : public IWorkingDaysWriter {
public:
    explicit QtWorkingDaysWriter(DBService& dbService);

    void changeWorkingDays(const WorkdayTracker& tracker) override;

    void addExtraHolidays(const std::vector<dw::Date>& days) override;

    void addExtraWorkdays(const std::vector<dw::Date>& days) override;

    void removeExtraHolidays(const std::vector<dw::Date>& days) override;

    void removeExtraWorkdays(const std::vector<dw::Date>& days) override;

private:
    DBService& dbService;
    qint64 storeSchedulesQueryId{-1};
    qint64 storeExceptionalDaysQuery{-1};

    void removeExtraDays(const std::vector<dw::Date>& days);
};

} // namespace sprint_timer::storage::qt_storage_impl

#endif /* end of include guard: QTWORKINGDAYSWRITER_H_GMTRHCHZ */
