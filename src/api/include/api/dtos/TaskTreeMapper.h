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
#ifndef TASKTREEMAPPER_H_3MWDRK5F
#define TASKTREEMAPPER_H_3MWDRK5F

#include "api/dtos/TaskTreeDTO.h"
#include "core/TaskTree.h"
#include "core/utils/Converter.h"

namespace sprint_timer::api {

class TaskTreeMapper : public Converter<TaskTreeDTO, TaskTree> {
public:
    explicit TaskTreeMapper(const Converter<TaskDTO, Task>& taskMapper);

private:
    [[nodiscard]] auto convert(const TaskTree& tree) const
        -> TaskTreeDTO override;

    [[nodiscard]] auto convert(const TaskTreeDTO& dto) const
        -> TaskTree override;

    const Converter<TaskDTO, Task>& taskMapper;
};

} // namespace sprint_timer::api

#endif /* end of include guard: TASKTREEMAPPER_H_3MWDRK5F */
