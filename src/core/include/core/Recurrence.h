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
#ifndef RECURRENCE_H_4H7M9DQE
#define RECURRENCE_H_4H7M9DQE

#include "core/DomainException.h"
#include "date_wrapper/date_wrapper.h"
#include <optional>
#include <string_view>

namespace sprint_timer {

class Recurrence {
public:
    explicit Recurrence(std::string recPattern);

    [[nodiscard]] auto nextRecurrence(dw::DateTime currentDateTime) const
        -> std::optional<dw::DateTime>;

    [[nodiscard]] auto pattern() const -> std::string;

    friend auto operator==(const Recurrence&,
                           const Recurrence&) -> bool = default;

private:
    std::string recPattern;
};

auto validateRecurrence(std::string_view pattern) -> void;

} // namespace sprint_timer

#endif /* end of include guard: RECURRENCE_H_4H7M9DQE */
