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
#ifndef PLANNEDITEM_H_ED4ZOXIL
#define PLANNEDITEM_H_ED4ZOXIL

#include "core/Recurrence.h"
#include <optional>

namespace sprint_timer {

class PlannedItem {
public:
    template <typename ItemT>
    PlannedItem(ItemT item_)
        : pimpl{std::make_unique<PlannedItemModel<ItemT>>(std::move(item_))}
    {
    }

private:
    struct PlannedItemConcept {
        virtual ~PlannedItemConcept() = default;

        virtual auto recurrence() const -> std::optional<Recurrence>;
    };

    template <typename ItemT> struct PlannedItemModel {
        explicit PlannedItemModel(ItemT item_)
            : item{std::move(item_)}
        {
        }
    };
};

} // namespace sprint_timer

#endif /* end of include guard: PLANNEDITEM_H_ED4ZOXIL */

