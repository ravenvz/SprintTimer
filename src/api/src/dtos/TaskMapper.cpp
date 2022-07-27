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
#include "api/dtos/TaskMapper.h"
#include "api/dtos/SprintMapper.h"
#include "api/dtos/TagMapper.h"
#include <algorithm>

namespace sprint_timer::api {

TaskDTO makeDTO(const sprint_timer::Task& task)
{
    std::vector<std::string> tags(task.tags().size());
    std::ranges::copy(dtoAdapter(task.tags()), begin(tags));
    std::vector<dw::DateTimeRange> sprints;
    sprints.reserve(task.sprints().size());
    std::ranges::copy(dtoAdapter(task.sprints()), std::back_inserter(sprints));
    return sprint_timer::api::TaskDTO{task.uuid(),
                                      tags,
                                      task.name(),
                                      task.estimatedCost(),
                                      sprints,
                                      task.isCompleted(),
                                      task.lastModified()};
}

Task fromDTO(const TaskDTO& dto)
{
    std::vector<Tag> tags;
    std::ranges::copy(dtoAdapter(dto.tags), std::back_inserter(tags));

    std::vector<Sprint> sprints;
    sprints.reserve(dto.sprints.size());
    std::ranges::copy(dtoAdapter(dto.sprints), std::back_inserter(sprints));

    return Task{dto.name,
                          dto.expectedCost,
                          sprints,
                          dto.uuid,
                          tags,
                          dto.finished,
                          dto.modificationStamp};
}

} // namespace sprint_timer::api

