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
#include "api/handlers/SprintsForTaskHandler.h"
#include "api/HandlerException.h"

namespace sprint_timer::api {

SprintsForTaskHandler::SprintsForTaskHandler(
    TaskStorageReader& reader_, const patterns::Converter<std::string, Tag>& tagMapper_)
    : reader{reader_}
    , tagMapper{tagMapper_}
{
}

SprintsForTaskQuery::Result
SprintsForTaskHandler::handle(const SprintsForTaskQuery& query)
{
    const auto tasks = reader.findByUuid(query.taskUuid);
    if (tasks.empty()) {
        throw HandlerException("unable to find task with uuid: " +
                               query.taskUuid);
    }
    const auto& sprints = tasks.front().sprints();

    // TODO see if we can move it somewhere (Task perhaps?)
    std::vector<std::string> tags;
    std::ranges::copy(tagMapper(tasks.front().tags()),
                      std::back_inserter(tags));

    std::vector<SprintDTO> res;
    res.reserve(sprints.size());

    std::ranges::transform(
        sprints, std::back_inserter(res), [&](const auto& sprint) {
            return SprintDTO{tasks.front().name(), tags, sprint.timeSpan()};
        });
    return res;
}

} // namespace sprint_timer::api

