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
#include "core/use_cases/SprintMapper.h"

namespace sprint_timer::use_cases {

SprintDTO makeDTO(const entities::Sprint& sprint)
{
    const auto& tagsEnt = sprint.tags();
    std::vector<std::string> tags(tagsEnt.size());
    std::transform(cbegin(tagsEnt),
                   cend(tagsEnt),
                   begin(tags),
                   [](const auto& elem) { return elem.name(); });
    return SprintDTO{sprint.uuid(),
                     sprint.taskUuid(),
                     sprint.name(),
                     tags,
                     sprint.timeSpan()};
}

entities::Sprint fromDTO(const SprintDTO& dto)
{
    const auto& tagStr = dto.tags;
    std::list<entities::Tag> tags(tagStr.size());
    std::transform(cbegin(tagStr),
                   cend(tagStr),
                   begin(tags),
                   [](const auto& elem) { return entities::Tag{elem}; });
    return entities::Sprint{
        dto.taskName, dto.timeRange, tags, dto.uuid, dto.taskUuid};
}

std::vector<SprintDTO> makeDTOs(const std::vector<entities::Sprint>& sprints)
{
    std::vector<SprintDTO> res;
    res.reserve(sprints.size());
    makeDTOs(cbegin(sprints), cend(sprints), std::back_inserter(res));
    return res;
}

std::vector<entities::Sprint> fromDTOs(const std::vector<SprintDTO>& dtos)
{
    std::vector<entities::Sprint> res;
    res.reserve(dtos.size());
    fromDTOs(cbegin(dtos), cend(dtos), std::back_inserter(res));
    return res;
}

} // namespace sprint_timer::use_cases
