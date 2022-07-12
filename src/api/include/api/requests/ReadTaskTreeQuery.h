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
#ifndef RETRIEVETASKTREE_H_IVJTJ9DF
#define RETRIEVETASKTREE_H_IVJTJ9DF

#include "api/dtos/TaskTreeDTO.h"

namespace sprint_timer::api {

struct ReadTaskTreeQuery {
    using Result = TaskTreeDTO;

    friend bool operator==(const ReadTaskTreeQuery&,
                           const ReadTaskTreeQuery&) = default;
};

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const ReadTaskTreeQuery&)
{
    os << "ReadTaskTreeQuery{}";
    return os;
}

} // namespace sprint_timer::api

#endif /* end of include guard: RETRIEVETASKTREE_H_IVJTJ9DF */
