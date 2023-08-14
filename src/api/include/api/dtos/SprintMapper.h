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
#ifndef SPRINTMAPPER_H_U4OQ1PMS
#define SPRINTMAPPER_H_U4OQ1PMS

#include "api/dtos/SprintDTO.h"
#include "core/Sprint.h"
#include "cpp_utils/patterns/Converter.h"
#include <ranges>
#include <span>

namespace sprint_timer::api {

class SprintMapper : public patterns::Converter<SprintDTO, SprintRecord> {
private:
    [[nodiscard]] auto make_dto_impl(const SprintRecord& sprintRecord) const
        -> SprintDTO override;

    [[nodiscard]] auto make_entity_impl(const SprintDTO& dto) const
        -> SprintRecord override;
};

class SprintDatetimeMapper
    : public patterns::Converter<dw::DateTimeRange, Sprint> {
private:
    [[nodiscard]] auto make_dto_impl(const Sprint& sprint) const
        -> dw::DateTimeRange override;

    [[nodiscard]] auto make_entity_impl(const dw::DateTimeRange& dto) const
        -> Sprint override;
};

} // namespace sprint_timer::api

#endif /* end of include guard: SPRINTMAPPER_H_U4OQ1PMS */
