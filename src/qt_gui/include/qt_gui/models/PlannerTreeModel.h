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
#ifndef PLANNERTREEMODEL_H_ETW7ZBLY
#define PLANNERTREEMODEL_H_ETW7ZBLY

#include "api/dtos/TaskTypeDTO.h"
#include "core/TreeType.h"
#include "qt_gui/models/CustomRoles.h"
#include "qt_gui/presentation/PlannerContract.h"
#include <QAbstractItemModel>
#include <QBrush>
#include <QIODevice>
#include <QIcon>
#include <QMimeData>

namespace sprint_timer::ui::qt_gui {

class PlannerTreeModel : public QAbstractItemModel {
public:
    explicit PlannerTreeModel(QObject* parent = nullptr);

    ~PlannerTreeModel() override;

    [[nodiscard]] auto supportedDropActions() const -> Qt::DropActions override;

    [[nodiscard]] auto supportedDragActions() const -> Qt::DropActions override;

    [[nodiscard]] auto mimeTypes() const -> QStringList override;

    [[nodiscard]] auto mimeData(const QModelIndexList& indexes) const
        -> QMimeData* override;

    auto dropMimeData(const QMimeData* data,
                      Qt::DropAction action,
                      int row,
                      int column,
                      const QModelIndex& parent) -> bool override;

    [[nodiscard]] auto rowCount(const QModelIndex& parent) const
        -> int override;

    [[nodiscard]] auto columnCount(const QModelIndex& parent) const
        -> int override;

    [[nodiscard]] auto flags(const QModelIndex& index) const
        -> Qt::ItemFlags override;

    [[nodiscard]] auto data(const QModelIndex& index, int role) const
        -> QVariant override;

    [[nodiscard]] auto
    index(int row, int column, const QModelIndex& parent) const
        -> QModelIndex override;

    [[nodiscard]] auto parent(const QModelIndex& index) const
        -> QModelIndex override;

    auto setData(const QModelIndex& index, const QVariant& value, int role)
        -> bool override;

    // auto headerData(int section, Qt::Orientation orientation, int role) const
    //     -> QVariant override;

    // auto moveRows(const QModelIndex& sourceParent,
    //               int sourceRow,
    //               int count,
    //               const QModelIndex& destinationParent,
    //               int destinationChild) -> bool override;

    // auto removeRows(int row, int count, const QModelIndex& parent)
    //     -> bool override;

    // auto insertRows(int row, int count, const QModelIndex& parent)
    //     -> bool override;

    // auto displayPlanner(
    //     const Tree<std::string, contracts::PlannerContract::PlannerItem>&
    //         taskTree) -> void;
    //
    // auto displayGoals() -> void;
    //
    // auto displayProjects() -> void;
    //
    // auto displayReviews() -> void;
private:
    struct ItemRepr {
        QString payload;
        QBrush foreground;
        QBrush background;
    };
    struct Item {
        std::string uuid;
        ItemRepr name;
        ItemRepr tags;
        ItemRepr progress;
        ItemRepr dueDate;
        QString notes;
        ItemRepr reminder;
        bool finished;
        bool recurrent;
        api::TaskTypeDTO type;
    };

    TreeType<Item> storage;

    auto getItemData(const QModelIndex& index, int role) const -> Item;

    auto replaceItemData(const QModelIndex& index, const Item& item, int role)
        -> bool;

    [[nodiscard]] auto
    makeItem(const contracts::PlannerContract::Item& plannerItem) const -> Item;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: PLANNERTREEMODEL_H_ETW7ZBLY */

