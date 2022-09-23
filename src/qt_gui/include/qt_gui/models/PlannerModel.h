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
#ifndef PLANNERMODEL_H_IUTH8WE0
#define PLANNERMODEL_H_IUTH8WE0

#include "qt_gui/TreeModel.h"
#include "qt_gui/presentation/PlannerContract.h"
#include <QAbstractItemModel>
#include <QColor>
#include <qmimedata.h>

namespace sprint_timer::ui::qt_gui {

class PlannerModel : public TreeModel {
public:
    explicit PlannerModel(QObject* parent = nullptr);

    auto supportedDropActions() const -> Qt::DropActions override;

    auto supportedDragActions() const -> Qt::DropActions override;

    auto mimeTypes() const -> QStringList override;

    auto mimeData(const QModelIndexList& indexes) const -> QMimeData* override;

    auto dropMimeData(const QMimeData* data,
                      Qt::DropAction action,
                      int row,
                      int column,
                      const QModelIndex& parent) -> bool override;

    auto flags(const QModelIndex& index) const -> Qt::ItemFlags override;

    // auto headerData(int section, Qt::Orientation orientation, int role) const
    //     -> QVariant override;

    auto data(const QModelIndex& index, int role) const -> QVariant override;

    // auto index(int row, int column, const QModelIndex& parent) const
    //     -> QModelIndex override;

    // auto parent(const QModelIndex& index) const -> QModelIndex override;

    // auto setData(const QModelIndex& index, const QVariant& value, int role)
    //     -> bool override;

    // auto moveRows(const QModelIndex& sourceParent,
    //               int sourceRow,
    //               int count,
    //               const QModelIndex& destinationParent,
    //               int destinationChild) -> bool override;

    // auto removeRows(int row, int count, const QModelIndex& parent)
    //     -> bool override;

    // auto insertRows(int row, int count, const QModelIndex& parent)
    //     -> bool override;

    // auto rowCount(const QModelIndex& parent) const -> int override;

    // auto columnCount(const QModelIndex& parent) const -> int override;

    auto displayPlanner(
        const Tree<std::string, contracts::PlannerContract::PlannerItem>&
            taskTree) -> void;

    auto displayGoals() -> void;

    auto displayProjects() -> void;

    auto displayReviews() -> void;

private:
    struct ItemRepr {
        QString payload;
        QColor pen;
        QColor brush;
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
        api::TaskTypeDTO type;
    };
    Tree<std::string, Item> storage;
    std::unique_ptr<Item> root;

    auto findParent(Item* node) const
        -> std::optional<std::pair<const Item*, int>>;

    auto makeItem(const contracts::PlannerContract::PlannerItem& dto) const
        -> Item;
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: PLANNERMODEL_H_IUTH8WE0 */
