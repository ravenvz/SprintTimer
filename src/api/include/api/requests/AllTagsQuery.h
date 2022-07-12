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
#ifndef ALLTAGSQUERY_H_OUBYGU0R
#define ALLTAGSQUERY_H_OUBYGU0R

#include "api/com_query/Query.h"
#include <string>
#include <vector>

namespace sprint_timer::api {

struct AllTagsQuery {
    using Result = std::vector<std::string>;

    friend bool operator==(const AllTagsQuery&, const AllTagsQuery&) = default;
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const AllTagsQuery&)
{
    os << "RequestAllTags{}";
    return os;
}

} // namespace sprint_timer::api

#endif /* end of include guard: ALLTAGSQUERY_H_OUBYGU0R */

