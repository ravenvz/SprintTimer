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
#ifndef TASKDTO_H_SZ7KJM8B
#define TASKDTO_H_SZ7KJM8B

#include "date_wrapper/date_wrapper.h"
#include <string>
#include <vector>

namespace sprint_timer::use_cases {

struct TaskDTO {
    std::string uuid;
    std::vector<std::string> tags;
    std::string name;
    int expectedCost;
    int actualCost;
    bool finished;
    dw::DateTime modificationStamp{dw::current_date_time_local()};

    friend bool operator==(const TaskDTO&, const TaskDTO&) = default;
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const TaskDTO& task)
{
    os << "TaskDTO{" << task.uuid << ", ";
    for (const auto& element : task.tags) {
        os << '#' << element << ' ';
    }
    os << task.actualCost << '/' << task.expectedCost;
    os << (task.finished ? " finished " : " pending ");
    os << task.modificationStamp << '}';
    return os;
}

} // namespace sprint_timer::use_cases

#endif /* end of include guard: TASKDTO_H_SZ7KJM8B */
