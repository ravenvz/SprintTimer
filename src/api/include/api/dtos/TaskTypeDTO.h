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
#ifndef TASKTYPEDTO_H_BCX1JA5R
#define TASKTYPEDTO_H_BCX1JA5R

#include <ostream>

namespace sprint_timer::api {

enum class TaskTypeDTO { Project, Folder, Regular };

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const TaskTypeDTO& dto)
{
    using enum TaskTypeDTO;
    switch (dto) {
    case Project:
        os << "Project";
        break;
    case Folder:
        os << "Folder";
        break;
    case Regular:
        os << "Regular";
    }
    os << "}";
    return os;
}

} // namespace sprint_timer::api

#endif /* end of include guard: TASKTYPEDTO_H_BCX1JA5R */
