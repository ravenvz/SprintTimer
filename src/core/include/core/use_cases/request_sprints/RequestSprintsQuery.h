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
#ifndef REQUESTSPRINTSQUERY_H_OFBUPL62
#define REQUESTSPRINTSQUERY_H_OFBUPL62

#include "core/Query.h"
#include "core/entities/Sprint.h"
#include <date_wrapper/date_wrapper.h>

namespace sprint_timer::use_cases {

struct RequestSprintsQuery : public Query<std::vector<entities::Sprint>> {
    RequestSprintsQuery(dw::DateRange dateRange);

    dw::DateRange dateRange;
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os,
           const RequestSprintsQuery& query)
{
    os << "RequestSprintsQuery {" << query.dateRange << "}";
    return os;
}

} // namespace sprint_timer::use_cases

#endif /* end of include guard: REQUESTSPRINTSQUERY_H_OFBUPL62 */

