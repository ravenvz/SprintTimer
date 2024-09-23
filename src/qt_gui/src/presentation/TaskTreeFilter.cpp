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
#include "qt_gui/presentation/TaskTreeFilter.h"
#include <iostream>

namespace sprint_timer::ui {

TaskTreeFilter::TaskTreeFilter(FilterMap&& filterMap_)
    : filterMap{std::move(filterMap_)}
{
}

auto TaskTreeFilter::operator()(const api::TaskTreeDTO& taskTree) const
    -> api::TaskTreeDTO
{
    if (not selectedFilter.has_value()) {
        return taskTree;
    }
    auto it = filterMap.find(*selectedFilter);
    if (it == cend(filterMap)) {
        std::cerr << "WARNING Ignoring Unknown task tree filter: "
                  << selectedFilter.value() << '\n';
        return taskTree;
    }
    return it->second(taskTree);
}

auto TaskTreeFilter::select(const FilterId& filter) -> void
{
    selectedFilter = filter;
    notify();
}

auto TaskTreeFilter::clear() -> void
{
    selectedFilter = std::nullopt;
    notify();
}

auto TaskTreeFilter::currentFilter() const -> std::optional<FilterId>
{
    return selectedFilter;
}

} // namespace sprint_timer::ui
