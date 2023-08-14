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
#include "core/Tag.h"
#include "cpp_utils/patterns/Converter.h"
#include <ranges>
#include <span>

namespace sprint_timer::api {

class TagMapper : public patterns::Converter<std::string, Tag> {
private:
    [[nodiscard]] auto make_dto_impl(const Tag& tag) const
        -> std::string override;

    [[nodiscard]] auto make_entity_impl(const std::string& dto) const
        -> Tag override;
};

} // namespace sprint_timer::api
