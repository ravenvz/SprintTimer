/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/

#include "core/use_cases/RequestFinishedTasks.h"

namespace UseCases {

RequestFinishedTasks::RequestFinishedTasks(
    ITaskStorageReader& taskStorageReader,
    const TimeSpan& timeSpan,
    ITaskStorageReader::Handler handler)
    : reader{taskStorageReader}
    , timeSpan{timeSpan}
    , handler{handler}
{
}

void RequestFinishedTasks::execute()
{
    reader.requestFinishedTasks(timeSpan, handler);
}

std::string RequestFinishedTasks::inspect() const
{
    std::stringstream ss;
    ss << "Request finished tasks in '" << timeSpan.toString("dd.MM.yyyy");
    return ss.str();
}


} /* UseCases */
