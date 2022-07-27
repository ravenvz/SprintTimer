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
#include "api/dtos/SprintMapper.h"

namespace sprint_timer::api {

auto makeDTO(const Sprint& sprint) -> dw::DateTimeRange
{
    return sprint.timeSpan();
}

auto fromDTO(const dw::DateTimeRange& dto) -> Sprint { return Sprint{dto}; }

auto makeDTO(const SprintRecord& sprint) -> SprintDTO
{
    const auto& tagsEnt = sprint.tags();
    std::vector<std::string> tags(tagsEnt.size());
    std::ranges::transform(
        tagsEnt, begin(tags), [](const auto& elem) { return elem.name(); });
    return SprintDTO{sprint.taskName(), tags, sprint.timeSpan()};
}

auto fromDTO(const SprintDTO& dto) -> SprintRecord
{
    const auto& tagStr = dto.tags;
    std::vector<Tag> tags(tagStr.size());
    std::ranges::transform(
        tagStr, begin(tags), [](const auto& elem) { return Tag{elem}; });
    return SprintRecord{dto.taskName, dto.timeRange, tags};
}

} // namespace sprint_timer::api
