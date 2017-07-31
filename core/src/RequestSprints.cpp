/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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

#include "core/use_cases/RequestSprints.h"

namespace UseCases {

RequestSprints::RequestSprints(
    ISprintStorageReader& reader,
    const dw::TimeSpan& timeSpan,
    std::function<void(const std::vector<Sprint>&)> resultHandler)
    : reader{reader}
    , timeSpan{timeSpan}
    , handler{resultHandler}
{
}

void RequestSprints::execute() { reader.requestItems(timeSpan, handler); }

std::string RequestSprints::inspect() const
{
    std::stringstream ss;
    ss << "Request sprints in '" << timeSpan.toString("dd.MM.yyyy");
    return ss.str();
}

} /* UseCases */
