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
#ifndef TASKTYPEMAPPER_H_QDECCH8H
#define TASKTYPEMAPPER_H_QDECCH8H

#include "api/dtos/TaskTypeDTO.h"
#include "core/TaskType.h"
#include "cpp_utils/patterns/Converter.h"

namespace sprint_timer::api {

class TaskTypeMapper : public patterns::Converter<TaskTypeDTO, TaskType> {
private:
    [[nodiscard]] auto make_dto_impl(const TaskType& kind) const
        -> TaskTypeDTO override;

    [[nodiscard]] auto make_entity_impl(const TaskTypeDTO& dto) const
        -> TaskType override;
};

} // namespace sprint_timer::api

#endif /* end of include guard: TASKTYPEMAPPER_H_QDECCH8H */
