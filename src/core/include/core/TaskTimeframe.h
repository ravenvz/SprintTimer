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
#ifndef TASKTIMEFRAME_H_TJRHDXEA
#define TASKTIMEFRAME_H_TJRHDXEA

#include "core/Recurrence.h"
#include "core/utils/Algutils.h"
#include "date_wrapper/date_wrapper.h"
#include <optional>

namespace sprint_timer {

struct TaskTimeframe {
    dw::DateTime start{dw::current_date_time_local()};
    std::optional<dw::DateTime> due;
    std::optional<dw::DateTime> remindAt;
    std::optional<Recurrence> recurrence;
};

inline auto operator==(const TaskTimeframe& lhs, const TaskTimeframe& rhs)
    -> bool
{
    auto date_time_equal = [](const auto& left, const auto& right) {
        return std::tuple(
                   left.date(), left.hour(), left.minute(), left.second()) ==
               std::tuple(
                   right.date(), right.hour(), right.minute(), right.second());
    };
    return date_time_equal(lhs.start, rhs.start) &&
           utils::opt_equal(lhs.due, rhs.due, date_time_equal) &&
           lhs.recurrence == rhs.recurrence &&
           utils::opt_equal(lhs.remindAt, rhs.remindAt, date_time_equal);
}

} // namespace sprint_timer

#endif /* end of include guard: TASKTIMEFRAME_H_TJRHDXEA */
