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

#include "core/use_cases/RequestFinishedTasks.h"

using dw::DateRange;

namespace sprint_timer::use_cases {

RequestFinishedTasks::RequestFinishedTasks(
    ITaskStorageReader& taskStorageReader,
    const DateRange& dateRange,
    ITaskStorageReader::Handler handler)
    : reader{taskStorageReader}
    , dateRange_{dateRange}
    , handler_{handler}
{
}

void RequestFinishedTasks::execute()
{
    reader.requestFinishedTasks(dateRange_, handler_);
}

std::string RequestFinishedTasks::describe() const
{
    std::stringstream ss;
    ss << "Request finished tasks in '"
       << dw::to_string(dateRange_, "dd.MM.yyyy");
    return ss.str();
}

} // namespace sprint_timer::use_cases
