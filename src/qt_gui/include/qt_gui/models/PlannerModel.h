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

#include "qt_gui/presentation/PlannerContract.h"
#include <QAbstractItemModel>

namespace sprint_timer::ui::qt_gui {

class PlannerModel : public QAbstractItemModel,
                     public contracts::PlannerContract::View {
public:
    PlannerModel(QObject* parent = nullptr);

    // Qt::DropActions supportedDropActions() const override
    // {
    //     return Qt::MoveAction;
    // }
    //
    // Qt::DropActions supportedDragActions() const override
    // {
    //     return Qt::MoveAction;
    // }
    //
    // Qt::ItemFlags flags(const QModelIndex& index) const override
    // {
    //     if (!index.isValid() || index.model() != this) {
    //         return Qt::ItemIsDropEnabled;
    //     }
    //     return QStandardItemModel::flags(index) | Qt::ItemIsUserCheckable |
    //            Qt::ItemIsDragEnabled;
    // }

    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role) const override;

    QVariant data(const QModelIndex& index, int role) const override;

    QModelIndex
    index(int row, int column, const QModelIndex& parent) const override;

    QModelIndex parent(const QModelIndex& index) const override;

    // bool setData(const QModelIndex& index,
    //              const QVariant& value,
    //              int role = Qt::EditRole) override
    // {
    //     return false;
    // }
    //
    // bool moveRows(const QModelIndex& sourceParent,
    //               int sourceRow,
    //               int count,
    //               const QModelIndex& destinationParent,
    //               int destinationChild) override
    // {
    //     return false;
    // }
    //
    // bool removeRows(int row,
    //                 int count,
    //                 const QModelIndex& parent = QModelIndex{}) override
    // {
    //     return false;
    // }
    //
    // bool insertRows(int row,
    //                 int count,
    //                 const QModelIndex& parent = QModelIndex{}) override
    // {
    //     return false;
    // }
    //
    int rowCount(const QModelIndex& parent) const override;

    int columnCount(const QModelIndex& parent) const override;

    void displayPlanner(const api::TaskTreeDTO& taskTree) override;

    void displayGoals();

    void displayProjects();

    void displayReviews();

private:
    class Item {
    public:
        Item(std::string uuid_,
             QString name_,
             int expected_,
             int actual_,
             bool finished_,
             QString tags_,
             std::optional<QDateTime> dueDate_,
             std::optional<QDateTime> reminder_,
             api::TaskTypeDTO type_,
             QString notes_);

        Item();

        ~Item();

        std::string uuid;
        QStringList data;
        bool finished{};
        api::TaskTypeDTO type{api::TaskTypeDTO::Regular};
        QString notes;
    };

    Tree<std::string, Item> storage;

    std::optional<std::pair<const Item*, int>> findParent(Item* node) const;

    friend Item toPlannerItem(const api::TaskNodeDTO& taskNode);
};

} // namespace sprint_timer::ui::qt_gui

#endif /* end of include guard: PLANNERMODEL_H_IUTH8WE0 */
