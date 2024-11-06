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

class TaskTimeframe {
public:
    constexpr TaskTimeframe() = default;

    constexpr TaskTimeframe(
        dw::DateTime startAt_,
        std::optional<dw::DateTime> dueTo_ = std::nullopt,
        std::optional<dw::DateTime> remindAt_ = std::nullopt,
        std::optional<Recurrence> recurr_ = std::nullopt)
        : startAt{startAt_}
        , dueTo{dueTo_}
        , remindAt{remindAt_}
        , recurr{std::move(recurr_)}
    {
        if (not dueTo) {
            dueTo = recurr.and_then(
                [&](const auto& rec) { return rec.nextRecurrence(*startAt); });
        }
    }

    constexpr TaskTimeframe(
        dw::DateTime startAt_,
        Recurrence recurr_,
        std::optional<dw::DateTime> remindAt_ = std::nullopt)
        : startAt{startAt_}
        , remindAt{remindAt_}
        , recurr{std::move(recurr_)}
    {
    }

    [[nodiscard]] constexpr auto
    inherit(const TaskTimeframe& other) const noexcept -> TaskTimeframe
    {
        return TaskTimeframe{other.start(), other.due()};
    }

    [[nodiscard]] constexpr auto start() const -> dw::DateTime
    {
        if (not startAt) {
            throw DomainException{"Timeframe must have start time"};
        }
        return *startAt;
    }

    [[nodiscard]] constexpr auto due() const -> std::optional<dw::DateTime>
    {
        return dueTo;
    }

    [[nodiscard]] constexpr auto reminder() const -> std::optional<dw::DateTime>
    {
        return remindAt;
    }

    [[nodiscard]] constexpr auto recurrence() const -> std::optional<Recurrence>
    {
        return recurr;
    }

private:
    std::optional<dw::DateTime> startAt{};
    // dw::DateTime startAt{dw::current_date_time_local()};
    std::optional<dw::DateTime> dueTo{};
    std::optional<dw::DateTime> remindAt{};
    std::optional<Recurrence> recurr{};
};

constexpr auto operator==(const TaskTimeframe& lhs,
                          const TaskTimeframe& rhs) -> bool
{
    auto date_time_equal = [](const auto& left, const auto& right) {
        return std::tuple(
                   left.date(), left.hour(), left.minute(), left.second()) ==
               std::tuple(
                   right.date(), right.hour(), right.minute(), right.second());
    };
    return date_time_equal(lhs.start(), rhs.start()) and
           alg::opt_equal(lhs.due(), rhs.due(), date_time_equal) and
           lhs.recurrence() == rhs.recurrence() and
           alg::opt_equal(lhs.reminder(), rhs.reminder(), date_time_equal);
}

inline auto format_datetime(const dw::DateTime& dt) -> std::string
{
    return dw::to_string(dt, "dd.MM.yyyy hh:mm:ss");
}

template <class CharT, class Traits>
std::basic_ostream<CharT, Traits>&
operator<<(std::basic_ostream<CharT, Traits>& os, const TaskTimeframe& frame)
{
    os << "TaskTimeframe{startAt: " << frame.start() << ", dueTo: "
       << ", dueTo: " << frame.due().transform(format_datetime).value_or("n/a")
       << ", remindAt: "
       << frame.reminder().transform(format_datetime).value_or("n/a")
       << ", recurr: "
       << frame.recurrence()
              .transform([](const auto& rec) { return rec.pattern(); })
              .value_or("n/a")
       << "}";
    return os;
}

} // namespace sprint_timer

#endif /* end of include guard: TASKTIMEFRAME_H_TJRHDXEA */
