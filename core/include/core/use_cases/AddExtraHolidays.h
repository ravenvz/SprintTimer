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
#ifndef ADDEXTRAHOLIDAYS_H_ZR08SNOQ
#define ADDEXTRAHOLIDAYS_H_ZR08SNOQ

#include <core/Command.h>
#include <core/IWorkingDaysWriter.h>

namespace sprint_timer::use_cases {

class AddExtraHolidays : public Command {
public:
    AddExtraHolidays(IWorkingDaysWriter& writer, const std::vector<dw::Date>& days);

    void execute() final;

    void undo() final;

    std::string describe() const final;

private:
    IWorkingDaysWriter& writer_;
    std::vector<dw::Date> days_;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: ADDEXTRAHOLIDAYS_H_ZR08SNOQ */
