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
#include "api/handlers/ReadTaskTreeHandler.h"
#include "core/SprintTimerException.h"

#include <iostream>

namespace sprint_timer::api {

ReadTaskTreeHandler::ReadTaskTreeHandler(
    TaskStorageReader& taskStorageReader_,
    const patterns::Converter<TaskTreeDTO, TaskTree>& taskTreeMapper_)
    : taskStorageReader{taskStorageReader_}
    , taskTreeMapper{taskTreeMapper_}
{
}

auto ReadTaskTreeHandler::handle(const ReadTaskTreeQuery& /*query*/)
    -> ReadTaskTreeQuery::Result
{
    return taskTreeMapper(taskStorageReader.taskTree());
}

} // namespace sprint_timer::api
