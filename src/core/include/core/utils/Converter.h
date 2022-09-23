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
#ifndef CONVERTER_H_HRJYFSDC
#define CONVERTER_H_HRJYFSDC

#include <ranges>
#include <span>

namespace sprint_timer {

template <typename From, typename To> class Converter {
public:
    virtual ~Converter() = default;

    [[nodiscard]] auto operator()(To const& entity) const -> From
    {
        return convert(entity);
    }

    [[nodiscard]] auto operator()(From const& dto) const -> To
    {
        return convert(dto);
    }

    [[nodiscard]] auto operator()(std::span<const To> entities) const
    {
        return std::views::transform(entities, [this](const auto& entity) {
            return operator()(entity);
        });
    }

    [[nodiscard]] auto operator()(std::span<const From> dtos) const
    {
        return std::views::transform(
            dtos, [this](const auto& dto) { return operator()(dto); });
    }

private:
    virtual auto convert(To const&) const -> From = 0;

    virtual auto convert(From const&) const -> To = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: CONVERTER_H_HRJYFSDC */
