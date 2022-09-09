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
#ifndef TASKTIMEFRAMEMAPPER_H_WJ31CXEL
#define TASKTIMEFRAMEMAPPER_H_WJ31CXEL

#include "api/dtos/TaskTimeframeDTO.h"
#include "core/TaskTimeframe.h"

namespace sprint_timer::api {

auto makeDTO(const TaskTimeframe& timeFrame) -> TaskTimeframeDTO;

auto fromDTO(const TaskTimeframeDTO& dto) -> TaskTimeframe;

} // namespace sprint_timer::api

#endif /* end of include guard: TASKTIMEFRAMEMAPPER_H_WJ31CXEL */
