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

#include "qt_gui/presentation/TaskDTO.h"
#include "core/entities/Task.h"

namespace sprint_timer::ui {

TaskDTO makeDTO(const entities::Task& task);

std::vector<TaskDTO> makeDTOs(const std::vector<entities::Task>& tasks);

entities::Task fromDTO(const TaskDTO& dto);

std::vector<entities::Task> fromDTOs(const std::vector<TaskDTO>& dtos);

} // namespace sprint_timer::ui

#endif /* end of include guard: TASKMAPPER_H_92B3YBWU */
