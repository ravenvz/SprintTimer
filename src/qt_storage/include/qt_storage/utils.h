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
#ifndef UTILS_H_IZ5NTG6C
#define UTILS_H_IZ5NTG6C

#include <algorithm>

namespace sprint_timer::storage::utils {

template <class InputIt1, class InputIt2, class OutputIt1, class OutputIt2>
void twoWayDiff(InputIt1 first1,
                InputIt1 last1,
                InputIt2 first2,
                InputIt2 last2,
                OutputIt1 out1,
                OutputIt2 out2)
{
    std::set_difference(first1, last1, first2, last2, out1);
    std::set_difference(first2, last2, first1, last1, out2);
}

} // namespace sprint_timer::storage::utils

#endif /* end of include guard: UTILS_H_IZ5NTG6C */
