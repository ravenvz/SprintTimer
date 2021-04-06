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
#ifndef SPRINTMAPPER_H_U4OQ1PMS
#define SPRINTMAPPER_H_U4OQ1PMS

#include "qt_gui/presentation/SprintDTO.h"
#include <core/entities/Sprint.h>

namespace sprint_timer::ui {

SprintDTO makeDTO(const entities::Sprint& sprint);

template <class InputIt, class OutputIt>
OutputIt makeDTOs(InputIt first, InputIt last, OutputIt out);

std::vector<SprintDTO> makeDTOs(const std::vector<entities::Sprint>& sprints);

entities::Sprint fromDTO(const SprintDTO& dto);

template <class InputIt, class OutputIt>
OutputIt fromDTOs(InputIt first, InputIt last, OutputIt out);

std::vector<entities::Sprint> fromDTOs(const std::vector<SprintDTO>& dtos);

template <class InputIt, class OutputIt>
inline OutputIt makeDTOs(InputIt first, InputIt last, OutputIt out)
{
    std::transform(
        first, last, out, [](const auto& elem) { return makeDTO(elem); });
    return out;
}

template <class InputIt, class OutputIt>
inline OutputIt fromDTOs(InputIt first, InputIt last, OutputIt out)
{
    std::transform(
        first, last, out, [](const auto& elem) { return fromDTO(elem); });
    return out;
}

} // namespace sprint_timer::ui

#endif /* end of include guard: SPRINTMAPPER_H_U4OQ1PMS */
