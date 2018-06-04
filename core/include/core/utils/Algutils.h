/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

#ifndef ALGUTILS_H_VLSTK4ZL
#define ALGUTILS_H_VLSTK4ZL

#include <algorithm>

namespace utils {

template <typename T>
auto slide(T first, T last, T position) -> std::pair<T, T>
{
    if (position < last)
        return {position, std::rotate(position, first, last)};
    if (last < position)
        return {std::rotate(first, last, position), position};
    return {first, last};
}

} // namespace utils

#endif /* end of include guard: ALGUTILS_H_VLSTK4ZL */
