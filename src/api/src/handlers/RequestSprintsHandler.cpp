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
#include "api/handlers/RequestSprintsHandler.h"
#include "api/dtos/SprintMapper.h"

namespace sprint_timer::api {

RequestSprintsHandler::RequestSprintsHandler(SprintStorageReader& reader_)
    : reader{reader_}
{
}

RequestSprintsQuery::Result
RequestSprintsHandler::handle(const RequestSprintsQuery& query)
{
    const auto sprintRecords = reader.findByDateRange(query.dateRange);
    std::vector<SprintDTO> res;
    res.reserve(sprintRecords.size());
    std::ranges::copy(dtoAdapter(sprintRecords), std::back_inserter(res));
    return res;
}

} // namespace sprint_timer::api

