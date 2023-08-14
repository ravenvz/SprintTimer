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
#include "api/dtos/TaskTypeMapper.h"
#include <algorithm>

namespace sprint_timer::api {

TaskMapper::TaskMapper(
    const patterns::Converter<NoteDTO, Note>& noteMapper_,
    const patterns::Converter<std::string, Tag>& tagMapper_,
    const patterns::Converter<TaskTimeframeDTO, TaskTimeframe>& timeFrameMapper_,
    const patterns::Converter<TaskTypeDTO, TaskType>& taskTypeMapper_,
    const patterns::Converter<dw::DateTimeRange, Sprint>& sprintMapper_)
    : noteMapper{noteMapper_}
    , tagMapper{tagMapper_}
    , timeFrameMapper{timeFrameMapper_}
    , taskTypeMapper{taskTypeMapper_}
    , sprintMapper{sprintMapper_}
{
}

auto TaskMapper::make_dto_impl(const Task& task) const -> TaskDTO
{
    std::vector<std::string> tags(task.tags().size());
    std::ranges::copy(tagMapper(task.tags()), begin(tags));
    std::vector<dw::DateTimeRange> sprints;
    sprints.reserve(task.sprints().size());
    std::ranges::copy(sprintMapper(task.sprints()),
                      std::back_inserter(sprints));
    const auto frame = timeFrameMapper(task.timeFrame());
    const auto notes = task.notes().transform(
        [this](const auto& nt) { return noteMapper(nt); });
    return api::TaskDTO{task.uuid(),
                        tags,
                        task.name(),
                        task.estimatedCost(),
                        sprints,
                        task.isCompleted(),
                        task.lastModified(),
                        notes,
                        frame,
                        taskTypeMapper(task.kind())};
}

auto TaskMapper::make_entity_impl(const TaskDTO& dto) const -> Task
{
    std::vector<Tag> tags;
    std::ranges::copy(tagMapper(dto.tags), std::back_inserter(tags));
    std::vector<Sprint> sprints;
    sprints.reserve(dto.sprints.size());
    std::ranges::copy(sprintMapper(dto.sprints), std::back_inserter(sprints));

    return Task{dto.name,
                dto.expectedCost,
                sprints,
                dto.uuid,
                tags,
                dto.finished,
                dto.modificationStamp,
                taskTypeMapper(dto.kind),
                dto.notes.transform(
                    [this](const auto& note) { return noteMapper(note); }),
                timeFrameMapper(dto.timeFrame)};
}

} // namespace sprint_timer::api

