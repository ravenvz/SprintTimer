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
#ifndef TREEMODEL_H_12NLRWHS
#define TREEMODEL_H_12NLRWHS

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>
#include <span>

namespace sprint_timer::ui::qt_gui {

class TreeItem;

class TreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    explicit TreeModel(std::span<const QString>, QObject* parent = nullptr);

    ~TreeModel() override;

    [[nodiscard]] auto data(const QModelIndex& index, int role) const
        -> QVariant override;

    [[nodiscard]] auto headerData(int section,
                                  Qt::Orientation orientation,
                                  int role = Qt::DisplayRole) const
        -> QVariant override;

    [[nodiscard]] auto
    index(int row, int column, const QModelIndex& parent = QModelIndex()) const
        -> QModelIndex override;

    [[nodiscard]] auto parent(const QModelIndex& index) const
        -> QModelIndex override;

    [[nodiscard]] auto rowCount(const QModelIndex& parent = QModelIndex()) const
        -> int override;

    [[nodiscard]] auto
    columnCount(const QModelIndex& parent = QModelIndex()) const
        -> int override;

    [[nodiscard]] auto flags(const QModelIndex& index) const
        -> Qt::ItemFlags override;

    auto setData(const QModelIndex& index,
                 const QVariant& value,
                 int role = Qt::EditRole) -> bool override;

    auto setHeaderData(int section,
                       Qt::Orientation orientation,
                       const QVariant& value,
                       int role = Qt::EditRole) -> bool override;

    auto insertColumns(int position,
                       int columns,
                       const QModelIndex& parent = QModelIndex())
        -> bool override;

    auto removeColumns(int position,
                       int columns,
                       const QModelIndex& parent = QModelIndex())
        -> bool override;

    auto insertRows(int position,
                    int rows,
                    const QModelIndex& parent = QModelIndex()) -> bool override;

    auto removeRows(int position,
                    int rows,
                    const QModelIndex& parent = QModelIndex()) -> bool override;

    auto moveRows(const QModelIndex& sourceParent,
                  int sourceRow,
                  int count,
                  const QModelIndex& destinationParent,
                  int destinationChild) -> bool;

private:
    std::unique_ptr<TreeItem> rootItem;

    [[nodiscard]] auto getItem(const QModelIndex& index) const -> TreeItem*;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: TREEMODEL_H_12NLRWHS */
