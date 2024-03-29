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
#include "qt_gui/models/TaskModel.h"
#include "core/utils/StringUtils.h"
#include "qt_gui/metatypes/TaskDTOMetatype.h"
#include "qt_gui/models/CustomRoles.h"
#include <string>

namespace {

std::string prefixTags(const std::vector<std::string>& tags);

} // namespace

namespace sprint_timer::ui::qt_gui {

TaskModel::TaskModel(QObject* parent_)
    : QAbstractListModel{parent_}
{
}

void TaskModel::displayTasks(const std::vector<use_cases::TaskDTO>& tasks)
{
    beginResetModel();
    storage = tasks;
    endResetModel();
}

Qt::DropActions TaskModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::DropActions TaskModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

Qt::ItemFlags TaskModel::flags(const QModelIndex& index) const
{
    if (!index.isValid() || index.model() != this) {
        return Qt::ItemIsDropEnabled;
    }
    return QAbstractListModel::flags(index) | Qt::ItemIsUserCheckable |
           Qt::ItemIsDragEnabled;
}

QVariant TaskModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const use_cases::TaskDTO& item = storage[static_cast<size_t>(index.row())];

    switch (role) {
    case Qt::DisplayRole:
        return QString{"%1 %2 %3/%4"}
            .arg(QString::fromStdString(prefixTags(item.tags)))
            .arg(QString::fromStdString(item.name))
            .arg(item.actualCost)
            .arg(item.expectedCost);

    case CustomRoles::ItemRole: {
        QVariant var;
        var.setValue(item);
        return var;
    }

    case CustomRoles::IdRole:
        return QString::fromStdString(item.uuid);

    case Qt::CheckStateRole:
        return item.finished;

    default:
        return QVariant();
    }
}

bool TaskModel::setData(const QModelIndex& index,
                        const QVariant& value,
                        int role)
{
    if (!index.isValid()) {
        return false;
    }

    const size_t pos{static_cast<size_t>(index.row())};

    switch (role) {

        // case static_cast<int>(TaskModelRoles::Insert): {
        //     if (index.row() != static_cast<int>(storage.size()) - 1) {
        //         return false;
        //     }
        //     storage[index.row()] = value.value<TaskDTO>();
        //     return true;
        // }

    case (CustomRoles::ReplaceRole): {
        if (auto p = presenter(); p) {
            const auto& oldTask = storage[pos];
            auto editedTask = value.value<use_cases::TaskDTO>();
            editedTask.uuid = oldTask.uuid;
            editedTask.actualCost = oldTask.actualCost;
            p.value()->editTask(editedTask);
            return true;
        }
        return false;
    }

    case static_cast<int>(CustomRoles::ToggleCheckedRole): {
        if (auto p = presenter(); p) {
            p.value()->toggleFinished(storage[pos].uuid,
                                      storage[pos].modificationStamp);
            return true;
        }
        return false;
    }

    default:
        return false;
    }
}

bool TaskModel::moveRows(const QModelIndex& /*sourceParent*/,
                         int sourceRow,
                         int count,
                         const QModelIndex& /*destinationParent*/,
                         int destinationChild)
{
    if (destinationChild == -1) {
        return false;
    }

    if (auto p = presenter(); p) {
        p.value()->reorderTasks(sourceRow, count, destinationChild);
        return true;
    }

    return false;
}

bool TaskModel::removeRows(int row, int count, const QModelIndex& /*parent*/)
{
    if (row < 0 || row >= static_cast<int>(storage.size())) {
        return false;
    }

    if (count != 1) {
        return false;
    }

    // Rely on presenter to resupply updated data (model data will be fully
    // replaced, hence no calls to begin/end|removeRows
    if (auto p = presenter(); p) {
        p.value()->deleteTask(storage[static_cast<size_t>(row)].uuid);
        return true;
    }
    return false;
}

bool TaskModel::insertRows(int /*row*/,
                           int /*count*/,
                           const QModelIndex& /*parent*/)
{
    // // Only support inserting one item after the last row
    // if (row != static_cast<int>(storage.size()) || count != 1) {
    //     return false;
    // }
    //
    // beginInsertRows(parent, rowCount(parent), rowCount(parent));
    // storage.emplace_back(TaskDTO{});
    // endInsertRows();
    // return true;

    return false;
}

int TaskModel::rowCount(const QModelIndex& /*parent*/) const
{
    return static_cast<int>(storage.size());
}

} // namespace sprint_timer::ui::qt_gui

namespace {

std::string prefixTags(const std::vector<std::string>& tags)
{
    return sprint_timer::utils::transformJoin(tags, " ", [](const auto& el) {
        std::string res{"#"};
        res += el;
        return res;
    });
}

} // namespace
