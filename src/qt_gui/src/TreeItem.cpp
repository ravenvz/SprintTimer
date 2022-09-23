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
#include "qt_gui/TreeItem.h"
#include "core/utils/Algutils.h"
#include <utility>

#include <iostream>

namespace sprint_timer::ui::qt_gui {

TreeItem::TreeItem(std::vector<QVariant> data_, TreeItem* parent_)
    : itemData{std::move(data_)}
    , parentItem{parent_}
{
}

auto TreeItem::child(int number) -> TreeItem*
{
    if (number < 0 || number >= static_cast<int>(childItems.size())) {
        return nullptr;
    }
    return childItems.at(static_cast<size_t>(number)).get();
}

auto TreeItem::childCount() const -> int
{
    return static_cast<int>(childItems.size());
}

auto TreeItem::childNumber() const -> int
{
    if (parentItem) {
        return static_cast<int>(
            std::distance(begin(parentItem->childItems),
                          std::ranges::find_if(parentItem->childItems,
                                               [this](const auto& ptr) {
                                                   return ptr.get() == this;
                                               })));
    }
    return 0;
}

auto TreeItem::columnCount() const -> int
{
    return static_cast<int>(itemData.size());
}

auto TreeItem::data(int column) const -> QVariant
{
    if (column < 0 || column >= static_cast<int>(itemData.size())) {
        return {};
    }
    return itemData.at(static_cast<size_t>(column));
}

auto TreeItem::insertRows(int position, int count, int columns) -> bool
{
    if (position < 0 || static_cast<size_t>(position) > childItems.size()) {
        return false;
    }

    auto make_empty_node = [&]() {
        return std::make_unique<TreeItem>(
            std::vector<QVariant>(static_cast<size_t>(columns)), this);
    };

    std::vector<std::unique_ptr<TreeItem>> values;
    values.reserve(static_cast<size_t>(count));
    std::ranges::generate_n(std::back_inserter(values), count, make_empty_node);
    childItems.insert(begin(childItems) + position,
                      std::make_move_iterator(begin(values)),
                      std::make_move_iterator(end(values)));

    return true;
}

auto TreeItem::insertColumns(int position, int columns) -> bool
{
    if (position < 0 || static_cast<size_t>(position) > itemData.size()) {
        return false;
    }

    itemData.insert(
        cbegin(itemData) + position, static_cast<size_t>(columns), QVariant{});

    for (auto& child : childItems) {
        child->insertColumns(position, columns);
    }

    return true;
}

auto TreeItem::parent() -> TreeItem* { return parentItem; }

auto TreeItem::removeRows(int position, int count) -> bool
{
    if (position < 0 ||
        position + count > static_cast<int>(childItems.size())) {
        return false;
    }

    childItems.erase(begin(childItems) + position,
                     begin(childItems) + position + count);

    return true;
}

auto TreeItem::removeColumns(int position, int columns) -> bool
{
    if (position < 0 ||
        position + columns > static_cast<int>(itemData.size())) {
        return false;
    }

    itemData.erase(begin(itemData) + position,
                   begin(itemData) + position + columns);

    for (auto& child : childItems) {
        child->removeColumns(position, columns);
    }

    return true;
}

auto TreeItem::setData(int column, const QVariant& value) -> bool
{
    if (column < 0 || static_cast<size_t>(column) >= itemData.size()) {
        return false;
    }

    itemData[static_cast<size_t>(column)] = value;
    return true;
}

auto moveRowsBetweenChildren(TreeItem* sourceParent,
                             int sourceRow,
                             int count,
                             TreeItem* destinationParent,
                             int destinationChild) -> bool
{
    if (sourceParent == destinationParent) {
        utils::slide(std::begin(sourceParent->childItems) + sourceRow,
                     std::begin(sourceParent->childItems) + sourceRow + count,
                     std::begin(sourceParent->childItems) + destinationChild);
        return true;
    }

    destinationParent->childItems.insert(
        begin(destinationParent->childItems) + destinationChild,
        std::make_move_iterator(begin(sourceParent->childItems) + sourceRow),
        std::make_move_iterator(begin(sourceParent->childItems) + sourceRow +
                                count));
    std::for_each(begin(destinationParent->childItems) + destinationChild,
                  begin(destinationParent->childItems) + destinationChild +
                      count,
                  [&](auto& child) { child->parentItem = destinationParent; });

    sourceParent->childItems.erase(begin(sourceParent->childItems) + sourceRow,
                                   begin(sourceParent->childItems) + sourceRow +
                                       count);
    return true;
}

} // namespace sprint_timer::ui::qt_gui
