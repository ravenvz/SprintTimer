/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "qt_gui/presentation/TaskMapper.h"
#include <algorithm>

namespace {

template <typename ForwardIt>
std::vector<std::string> tagsToString(ForwardIt first, ForwardIt last)
{
    std::vector<std::string> strings(
        static_cast<size_t>(std::distance(first, last)));
    std::transform(first, last, begin(strings), [](const auto& elem) {
        return elem.name();
    });
    return strings;
}

template <typename ForwardIt>
std::list<sprint_timer::entities::Tag> tagsFromStrings(ForwardIt first,
                                                       ForwardIt last)
{
    std::list<sprint_timer::entities::Tag> tags(
        static_cast<size_t>(std::distance(first, last)));
    std::transform(first, last, begin(tags), [](const auto& elem) {
        return sprint_timer::entities::Tag{elem};
    });
    return tags;
}

} // namespace

namespace sprint_timer::ui {

sprint_timer::ui::TaskDTO makeDTO(const sprint_timer::entities::Task& task)
{
    const auto& tags = task.tags();
    std::vector<std::string> tagStr = tagsToString(cbegin(tags), cend(tags));
    return sprint_timer::ui::TaskDTO{task.uuid(),
                                     tagStr,
                                     task.name(),
                                     task.estimatedCost(),
                                     task.actualCost(),
                                     task.isCompleted(),
                                     task.lastModified()};
}

std::vector<TaskDTO> makeDTOs(const std::vector<entities::Task>& tasks)
{
    std::vector<TaskDTO> dtos;
    dtos.reserve(tasks.size());
    std::transform(cbegin(tasks),
                   cend(tasks),
                   std::back_inserter(dtos),
                   [](const auto& elem) { return makeDTO(elem); });
    return dtos;
}

entities::Task fromDTO(const TaskDTO& dto)
{
    return entities::Task{dto.name,
                          dto.expectedCost,
                          dto.actualCost,
                          dto.uuid,
                          tagsFromStrings(cbegin(dto.tags), cend(dto.tags)),
                          dto.finished,
                          dto.modificationStamp};
}

std::vector<entities::Task> fromDTOs(const std::vector<TaskDTO>& dtos)
{
    std::vector<entities::Task> tasks(dtos.size());
    std::transform(cbegin(dtos),
                   cend(dtos),
                   begin(tasks),
                   [](const auto& elem) { return fromDTO(elem); });
    return tasks;
}

} // namespace sprint_timer::ui
