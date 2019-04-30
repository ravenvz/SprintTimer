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
#include "core/use_cases/ChangeWorkingDays.h"

namespace sprint_timer::use_cases {

ChangeWorkingDays::ChangeWorkingDays(IWorkingDaysWriter& writer_,
                                     const WorkdayTracker& oldTracker_,
                                     const WorkdayTracker& newTracker_)
    : writer{writer_}
    , oldTracker{oldTracker_}
    , newTracker{newTracker_}
{
}

void ChangeWorkingDays::execute() { writer.changeWorkingDays(newTracker); }

void ChangeWorkingDays::undo() { writer.changeWorkingDays(oldTracker); }

std::string ChangeWorkingDays::describe() const
{
    std::stringstream ss;
    ss << "Change workdays \nfrom:\n";
    ss << oldTracker;
    ss << "\nto:\n";
    ss << newTracker << "\n";
    return ss.str();
}

} // namespace sprint_timer::use_cases
