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

#include "date_wrapper/date_wrapper.h"

namespace sprint_timer {

class Recurrence {
public:
    explicit Recurrence(std::string recPattern);

    [[nodiscard]] auto nextRecurrence() const -> dw::DateTime;

    [[nodiscard]] auto pattern() const -> std::string;

private:
    std::string recPattern;
};

inline Recurrence::Recurrence(std::string recPattern_)
    : recPattern{std::move(recPattern_)}
{
}

inline auto Recurrence::nextRecurrence() const -> dw::DateTime
{
    return dw::current_date_time_local();
}

inline auto Recurrence::pattern() const -> std::string { return recPattern; }

} // namespace sprint_timer

#endif /* end of include guard: RECURRENCE_H_4H7M9DQE */
