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
#ifndef TASKMAPPER_H_92B3YBWU
#define TASKMAPPER_H_92B3YBWU

#include "api/dtos/NoteMapper.h"
#include "api/dtos/TaskDTO.h"
#include "core/Task.h"
#include "cpp_utils/patterns/Converter.h"
#include <ranges>
#include <span>

namespace sprint_timer::api {

class TaskMapper : public patterns::Converter<TaskDTO, Task> {
public:
    TaskMapper(
        const patterns::Converter<NoteDTO, Note>& noteMapper,
        const patterns::Converter<std::string, Tag>& tagMapper,
        const patterns::Converter<TaskTimeframeDTO, TaskTimeframe>&
            timeFrameMapper_,
        const patterns::Converter<TaskTypeDTO, TaskType>& taskTypeMapper_,
        const patterns::Converter<dw::DateTimeRange, Sprint>& sprintMapper);

private:
    [[nodiscard]] auto make_dto_impl(const Task& task) const
        -> TaskDTO override;

    [[nodiscard]] auto make_entity_impl(const TaskDTO& dto) const
        -> Task override;

    const patterns::Converter<NoteDTO, Note>& noteMapper;
    const patterns::Converter<std::string, Tag>& tagMapper;
    const patterns::Converter<TaskTimeframeDTO, TaskTimeframe>& timeFrameMapper;
    const patterns::Converter<TaskTypeDTO, TaskType>& taskTypeMapper;
    const patterns::Converter<dw::DateTimeRange, Sprint>& sprintMapper;
};

} // namespace sprint_timer::api

#endif /* end of include guard: TASKMAPPER_H_92B3YBWU */
