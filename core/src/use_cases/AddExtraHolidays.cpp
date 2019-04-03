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
#include "core/use_cases/AddExtraHolidays.h"

namespace sprint_timer::use_cases {

AddExtraHolidays::AddExtraHolidays(IWorkingDaysWriter& writer,
                                 const std::vector<dw::Date>& days)
    : writer_{writer}
    , days_{days}
{
}

void AddExtraHolidays::execute() { writer_.addExtraHolidays(days_); }

void AddExtraHolidays::undo() { writer_.removeExtraHolidays(days_); }

std::string AddExtraHolidays::describe() const
{
    std::stringstream ss;
    ss << "Add extra holidays:\n";
    for (const auto& day : days_) {
        ss << "\t" << dw::to_string(day, "dd.MM.yyyy");
    }
    return ss.str();
}

} // namespace sprint_timer::use_cases
