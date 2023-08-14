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

auto SprintMapper::make_dto_impl(const SprintRecord& sprintRecord) const -> SprintDTO
{
    const auto& tagsEnt = sprintRecord.tags();
    std::vector<std::string> tags(tagsEnt.size());
    std::ranges::transform(
        tagsEnt, begin(tags), [](const auto& elem) { return elem.name(); });
    return SprintDTO{sprintRecord.taskName(), tags, sprintRecord.timeSpan()};
}

auto SprintMapper::make_entity_impl(const SprintDTO& dto) const -> SprintRecord
{
    const auto& tagStr = dto.tags;
    std::vector<Tag> tags(tagStr.size());
    std::ranges::transform(
        tagStr, begin(tags), [](const auto& elem) { return Tag{elem}; });
    return SprintRecord{dto.taskName, dto.timeRange, tags};
}

auto SprintDatetimeMapper::make_dto_impl(const Sprint& sprint) const
    -> dw::DateTimeRange
{
    return sprint.timeSpan();
}

auto SprintDatetimeMapper::make_entity_impl(const dw::DateTimeRange& dto) const
    -> Sprint
{
    return Sprint{dto};
}

} // namespace sprint_timer::api
