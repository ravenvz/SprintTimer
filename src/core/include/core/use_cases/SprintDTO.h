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
#ifndef SPRINTDTO_H_VMXFAC02
#define SPRINTDTO_H_VMXFAC02

#include "core/Uuid.h"
#include "date_wrapper/date_wrapper.h"
#include <string>
#include <vector>

namespace sprint_timer::use_cases {

struct SprintDTO {
    Uuid uuid;
    Uuid taskUuid;
    std::string taskName;
    std::vector<std::string> tags;
    dw::DateTimeRange timeRange{dw::current_date_time(),
                                dw::current_date_time()};

    bool operator==(const SprintDTO& other) const = default;
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const SprintDTO& sprint)
{
    os << "SprintDTO{"
       << "taskUuid: " << sprint.taskUuid << ", " << sprint.taskName << ", "
       << sprint.uuid;
    for (const auto& element : sprint.tags) {
        os << '#' << element << ' ';
    }
    os << sprint.timeRange << "}";
    return os;
}

} // namespace sprint_timer::use_cases

#endif /* end of include guard: SPRINTDTO_H_VMXFAC02 */
