/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "core/actions/ChangeTasksPriorities.h"

namespace sprint_timer::actions {

ChangeTasksPriorities::ChangeTasksPriorities(
    TaskStorageWriter& writer_,
    std::vector<std::string>&& old_order,
    std::vector<std::string>&& new_order)
    : writer{writer_}
    , old_order_{std::move(old_order)}
    , new_order_{std::move(new_order)}
{
}

void ChangeTasksPriorities::execute()
{
    writer.updatePriorities(std::move(new_order_));
}

void ChangeTasksPriorities::undo()
{
    writer.updatePriorities(std::move(old_order_));
}

std::string ChangeTasksPriorities::describe() const
{
    return "Store unfinished tasks order";
}

} // namespace sprint_timer::actions
