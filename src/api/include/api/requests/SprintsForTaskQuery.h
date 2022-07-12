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
#ifndef SPRINTSFORTASKQUERY_H_VJBIMPU0
#define SPRINTSFORTASKQUERY_H_VJBIMPU0

#include "api/dtos/SprintDTO.h"

namespace sprint_timer::api {

struct SprintsForTaskQuery {

    using Result = std::vector<SprintDTO>;

    std::string taskUuid;

    friend bool operator==(const SprintsForTaskQuery&,
                           const SprintsForTaskQuery&) = default;
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const SprintsForTaskQuery& query)
{
    os << "SprintsForTaskQuery{" << query.taskUuid << "}";
    return os;
}

} // namespace sprint_timer::api

#endif /* end of include guard: SPRINTSFORTASKQUERY_H_VJBIMPU0 */
