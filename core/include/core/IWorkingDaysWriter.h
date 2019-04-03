/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#ifndef IWORKINGDAYWRITER_H_MX1NQTVU
#define IWORKINGDAYWRITER_H_MX1NQTVU

#include "core/WorkdayTracker.h"

namespace sprint_timer {

class IWorkingDaysWriter {
public:
    virtual ~IWorkingDaysWriter() = default;

    virtual void addExtraHolidays(const std::vector<dw::Date>& days) = 0;

    virtual void addExtraWorkdays(const std::vector<dw::Date>& days) = 0;

    virtual void removeExtraHolidays(const std::vector<dw::Date>& days) = 0;

    virtual void removeExtraWorkdays(const std::vector<dw::Date>& days) = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: IWORKINGDAYWRITER_H_MX1NQTVU */
