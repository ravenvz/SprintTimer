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
#include "qt_gui/TreeModel.h"
#include "qt_gui/TreeItem.h"
#include <QtWidgets>

namespace sprint_timer::ui::qt_gui {

TreeModel::TreeModel(std::span<const QString> headers_, QObject* parent_)
    : QAbstractItemModel{parent_}
{
    std::vector<QVariant> rootData;
    rootData.reserve(headers_.size());
    std::ranges::transform(headers_,
                           std::back_inserter(rootData),
                           [](const auto header) { return QVariant{header}; });
    rootItem = std::make_unique<TreeItem>(rootData);
}

TreeModel::~TreeModel() = default;

auto TreeModel::columnCount(const QModelIndex& /* parent */) const -> int
{
    return rootItem->columnCount();
}

auto TreeModel::data(const QModelIndex& index, int role) const -> QVariant
{
    if (!index.isValid()) {
        return {};
    }

    TreeItem* item = getItem(index);
    return item->data(index.column());
}

auto TreeModel::flags(const QModelIndex& index) const -> Qt::ItemFlags
{
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}

auto TreeModel::getItem(const QModelIndex& index) const -> TreeItem*
{
    if (index.isValid()) {
        auto* item = static_cast<TreeItem*>(index.internalPointer());
        if (item != nullptr) {
            return item;
        }
    }
    return rootItem.get();
}

auto TreeModel::headerData(int section,
                           Qt::Orientation orientation,
                           int role) const -> QVariant
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return rootItem->data(section);
    }

    return {};
}

auto TreeModel::index(int row, int column, const QModelIndex& parent) const
    -> QModelIndex
{
    if (parent.isValid() && parent.column() != 0) {
        return {};
    }

    TreeItem* parentItem = getItem(parent);
    if (parentItem == nullptr) {
        return {};
    }

    TreeItem* childItem = parentItem->child(row);
    if (childItem != nullptr) {
        return createIndex(row, column, childItem);
    }
    return {};
}

auto TreeModel::insertColumns(int position,
                              int columns,
                              const QModelIndex& parent) -> bool
{
    beginInsertColumns(parent, position, position + columns - 1);
    const bool success = rootItem->insertColumns(position, columns);
    endInsertColumns();

    return success;
}

auto TreeModel::insertRows(int position, int rows, const QModelIndex& parent)
    -> bool
{
    TreeItem* parentItem = getItem(parent);
    if (parentItem == nullptr) {
        return false;
    }

    beginInsertRows(parent, position, position + rows - 1);
    const bool success =
        parentItem->insertRows(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}

auto TreeModel::parent(const QModelIndex& index) const -> QModelIndex
{
    if (!index.isValid()) {
        return {};
    }

    TreeItem* childItem = getItem(index);
    TreeItem* parentItem = childItem ? childItem->parent() : nullptr;

    if (parentItem == rootItem.get() || !parentItem) {
        return {};
    }

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

auto TreeModel::removeColumns(int position,
                              int columns,
                              const QModelIndex& parent) -> bool
{
    beginRemoveColumns(parent, position, position + columns - 1);
    const bool success = rootItem->removeColumns(position, columns);
    endRemoveColumns();

    if (rootItem->columnCount() == 0) {
        removeRows(0, rowCount());
    }

    return success;
}

auto TreeModel::removeRows(int position, int rows, const QModelIndex& parent)
    -> bool
{
    TreeItem* parentItem = getItem(parent);
    if (!parentItem) {
        return false;
    }

    beginRemoveRows(parent, position, position + rows - 1);
    const bool success = parentItem->removeRows(position, rows);
    endRemoveRows();

    return success;
}

auto TreeModel::moveRows(const QModelIndex& sourceParent,
                         int sourceRow,
                         int count,
                         const QModelIndex& destinationParent,
                         int destinationChild) -> bool
{
    if (beginMoveRows(sourceParent,
                      sourceRow,
                      sourceRow + count - 1,
                      destinationParent,
                      destinationChild)) {

        auto* parent = getItem(sourceParent);
        if (parent == nullptr) {
            throw std::runtime_error{"parent is null"};
        }
        auto* destination = getItem(destinationParent);
        if (destination == nullptr) {
            throw std::runtime_error{"destination is null"};
        }

        qDebug() << sourceParent.data(Qt::DisplayRole);
        qDebug() << destinationParent.data(Qt::DisplayRole);
        qDebug() << parent->data(0);
        qDebug() << destination->data(0);

        moveRowsBetweenChildren(
            parent, sourceRow, count, destination, destinationChild);
        endMoveRows();
        return true;
    }
    return false;
}

auto TreeModel::rowCount(const QModelIndex& parent) const -> int
{
    if (parent.isValid() && parent.column() > 0) {
        return 0;
    }

    const auto* parentItem = getItem(parent);
    return parentItem ? parentItem->childCount() : 0;
}

auto TreeModel::setData(const QModelIndex& index,
                        const QVariant& value,
                        int role) -> bool
{
    TreeItem* item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result) {
        emit dataChanged(index, index, {role});
    }

    return result;
}

bool TreeModel::setHeaderData(int section,
                              Qt::Orientation orientation,
                              const QVariant& value,
                              int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal) {
        return false;
    }

    const bool result = rootItem->setData(section, value);

    if (result) {
        emit headerDataChanged(orientation, section, section);
    }

    return result;
}

} // namespace sprint_timer::ui::qt_gui
