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
#ifndef TREEITEM_H_N7LCZY5G
#define TREEITEM_H_N7LCZY5G

#include <QVariant>
#include <QVector>
#include <span>

namespace sprint_timer::ui::qt_gui {

class TreeItem {
public:
    explicit TreeItem(std::vector<QVariant> data, TreeItem* parent = nullptr);

    TreeItem(TreeItem&&) = default;

    auto operator=(TreeItem&&) -> TreeItem& = default;

    auto operator=(const TreeItem&) -> TreeItem& = delete;

    TreeItem(const TreeItem&) = delete;

    auto child(int number) -> TreeItem*;

    [[nodiscard]] auto childCount() const -> int;

    [[nodiscard]] auto columnCount() const -> int;

    [[nodiscard]] auto data(int column) const -> QVariant;

    auto insertRows(int position, int count, int columns) -> bool;

    auto insertColumns(int position, int columns) -> bool;

    auto parent() -> TreeItem*;

    auto removeRows(int position, int count) -> bool;

    auto removeColumns(int position, int columns) -> bool;

    [[nodiscard]] auto childNumber() const -> int;

    auto setData(int column, const QVariant& value) -> bool;

    friend auto moveRowsBetweenChildren(TreeItem* sourceParent,
                                        int sourceRow,
                                        int count,
                                        TreeItem* destinationParent,
                                        int destinationChild) -> bool;

private:
    std::vector<std::unique_ptr<TreeItem>> childItems;
    std::vector<QVariant> itemData;
    TreeItem* parentItem;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: TREEITEM_H_N7LCZY5G */
