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
#ifndef TASKSINRANGEQUERY_H_QKKGI7SR
#define TASKSINRANGEQUERY_H_QKKGI7SR

#include "api/com_query/Query.h"
#include "api/dtos/TaskDTO.h"

namespace sprint_timer::api {

struct TasksInRangeQuery : public asp::Query<std::vector<TaskDTO>> {
    explicit TasksInRangeQuery(dw::DateRange dateRange_)
        : dateRange{dateRange_}
    {
    }

    friend bool operator==(const TasksInRangeQuery&,
                           const TasksInRangeQuery&) = default;

    dw::DateRange dateRange;
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const TasksInRangeQuery& query)
{
    os << "TasksInRangeQuery{" << query.dateRange << "}";
    return os;
}

} // namespace sprint_timer::api

#endif /* end of include guard: TASKSINRANGEQUERY_H_QKKGI7SR */
