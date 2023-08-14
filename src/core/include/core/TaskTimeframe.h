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
#include "cpp_utils/algorithms/optional_ext.h"
#include "date_wrapper/date_wrapper.h"
#include <optional>

namespace sprint_timer {

struct TaskTimeframe {
    constexpr TaskTimeframe() = default;

    constexpr TaskTimeframe(
        dw::DateTime start_,
        std::optional<dw::DateTime> due_ = std::nullopt,
        std::optional<dw::DateTime> remindAt_ = std::nullopt,
        std::optional<Recurrence> recurrence_ = std::nullopt)
        : start{start_}
        , due{due_}
        , remindAt{remindAt_}
        , recurrence{std::move(recurrence_)}
    {
        if (not due) {
            due = recurrence.and_then(
                [&](const auto& rec) { return rec.nextRecurrence(start); });
        }
    }

    constexpr TaskTimeframe(
        dw::DateTime start_,
        Recurrence recurrence_,
        std::optional<dw::DateTime> remindAt_ = std::nullopt)
        : start{start_}
        , remindAt{remindAt_}
        , recurrence{std::move(recurrence_)}
    {
    }

    constexpr auto inherit(const TaskTimeframe& other) const noexcept
        -> TaskTimeframe
    {
        return TaskTimeframe{other.start, other.due};
    }

    dw::DateTime start{dw::current_date_time_local()};
    std::optional<dw::DateTime> due{};
    std::optional<dw::DateTime> remindAt{};
    std::optional<Recurrence> recurrence{};
};

constexpr auto operator==(const TaskTimeframe& lhs, const TaskTimeframe& rhs)
    -> bool
{
    auto date_time_equal = [](const auto& left, const auto& right) {
        return std::tuple(
                   left.date(), left.hour(), left.minute(), left.second()) ==
               std::tuple(
                   right.date(), right.hour(), right.minute(), right.second());
    };
    return date_time_equal(lhs.start, rhs.start) &&
           alg::opt_equal(lhs.due, rhs.due, date_time_equal) &&
           lhs.recurrence == rhs.recurrence &&
           alg::opt_equal(lhs.remindAt, rhs.remindAt, date_time_equal);
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const TaskTimeframe& frame)
{
    os << "TaskTimeframe{start: " << frame.start << ", due: "
       << frame.due
              .transform([](const auto& dt) {
                  return dw::to_string(dt, "dd.MM.yyyy hh:mm:ss");
              })
              .value_or("n/a")
       << ", remindAt: "
       << frame.remindAt
              .transform([](const auto& dt) {
                  return dw::to_string(dt, "dd.MM.yyyy hh:mm:ss");
              })
              .value_or("n/a")
       << ", recurrence: "
       << frame.recurrence
              .transform([](const auto& rec) { return rec.pattern(); })
              .value_or("n/a")
       << "}";
    return os;
}

} // namespace sprint_timer

#endif /* end of include guard: TASKTIMEFRAME_H_TJRHDXEA */
