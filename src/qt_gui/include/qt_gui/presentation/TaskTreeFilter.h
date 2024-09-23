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
#ifndef TASKTREEFILTER_H_ZRBSFCL4
#define TASKTREEFILTER_H_ZRBSFCL4

#include "api/dtos/TaskTreeDTO.h"
#include "core/Observable.h"

#include <optional>
#include <unordered_map>

namespace sprint_timer::ui {

class TaskTreeFilter : public Observable {
public:
    using FilterId = std::string;
    using Filter = std::function<api::TaskTreeDTO(const api::TaskTreeDTO&)>;
    using FilterMap = std::unordered_map<FilterId, Filter>;

    explicit TaskTreeFilter(FilterMap&& filterMap);

    auto operator()(const api::TaskTreeDTO& taskTree) const -> api::TaskTreeDTO;

    auto select(const FilterId& filter) -> void;

    auto currentFilter() const -> std::optional<FilterId>;

    auto clear() -> void;

private:
    FilterMap filterMap;
    std::optional<FilterId> selectedFilter;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: TASKTREEFILTER_H_ZRBSFCL4 */
