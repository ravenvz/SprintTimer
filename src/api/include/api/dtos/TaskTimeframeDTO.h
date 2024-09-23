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
#ifndef TASKTIMEFRAMEDTO_H_Z5XCFSPI
#define TASKTIMEFRAMEDTO_H_Z5XCFSPI

#include "cpp_utils/algorithms/optional_ext.h"
#include "date_wrapper/date_wrapper.h"
#include <optional>
#include <string>

namespace sprint_timer::api {

struct TaskTimeframeDTO {
    dw::DateTime start{dw::current_date_time_local()};
    std::optional<dw::DateTime> due;
    std::optional<dw::DateTime> remindAt{};
    std::optional<std::string> recurrence{};
};

inline auto operator==(const TaskTimeframeDTO& lhs,
                       const TaskTimeframeDTO& rhs) -> bool
{
    auto date_time_equal = [](const auto& left, const auto& right) {
        return std::tuple(
                   left.date(), left.hour(), left.minute(), left.second()) ==
               std::tuple(
                   right.date(), right.hour(), right.minute(), right.second());
    };
    return date_time_equal(lhs.start, rhs.start) &&
           ((!lhs.due && !rhs.due) ||
            (lhs.due && rhs.due && date_time_equal(*lhs.due, *rhs.due))) &&
           lhs.recurrence == rhs.recurrence &&
           ((!lhs.remindAt && !rhs.remindAt) ||
            (lhs.remindAt && rhs.remindAt &&
             date_time_equal(*lhs.remindAt, *rhs.remindAt)));
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const TaskTimeframeDTO& frame)
{
    os << "TimeframeDTO{";
    os << "start: " << frame.start << " ";
    alg::inspect(frame.due, [&](const auto& x) { os << "due: " << x << " "; });
    alg::inspect(frame.remindAt,
                 [&](const auto& x) { os << "remind: " << x << " "; });
    alg::inspect(frame.recurrence,
                 [&](const auto& x) { os << "recurrence: " << x; });
    os << "}";
    return os;
}

} // namespace sprint_timer::api

#endif /* end of include guard: TASKTIMEFRAMEDTO_H_Z5XCFSPI */
