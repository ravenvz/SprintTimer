/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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
#include "TaskModel.h"
#include <QSize>
#include <algorithm>
#include <iostream>

TaskModel::TaskModel(ICoreService& coreService, QObject* parent)
    : AsyncListModel{parent}
    , coreService{coreService}
{
    synchronize();
}

void TaskModel::requestDataUpdate()
{
    coreService.requestUnfinishedTasks(
        std::bind(&TaskModel::onDataChanged, this, std::placeholders::_1));
}

void TaskModel::onDataChanged(const std::vector<Task>& tasks)
{
    beginResetModel();
    storage = tasks;
    endResetModel();
    broadcastUpdateFinished();
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
    return QAbstractListModel::flags(index) | Qt::ItemIsUserCheckable
        | Qt::ItemIsDragEnabled;
}

QVariant TaskModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) {
        return QVariant();
    }

    const Task& taskRef = storage[index.row()];

    switch (role) {
    case TagsRole:
        return QString::fromStdString(prefixTags(taskRef.tags()));
    case DescriptionRole:
        return QString::fromStdString(taskRef.name());
    case StatsRole:
        return QString("%1/%2")
            .arg(taskRef.actualCost())
            .arg(taskRef.estimatedCost());
    case Qt::CheckStateRole:
        return taskRef.isCompleted();
    case GetFinishedSprintsRole:
        return taskRef.actualCost();
    default:
        return QVariant();
    }
}

int TaskModel::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(storage.size());
}

void TaskModel::insert(const Task& item)
{
    coreService.registerTask(item);
    requestDataUpdate();
}

void TaskModel::remove(const QModelIndex& index) { remove(index.row()); }

void TaskModel::remove(const int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    coreService.removeTask(itemAt(row));
    requestDataUpdate();
    endRemoveRows();
}

Task TaskModel::itemAt(const int row) const { return storage.at(row); }

void TaskModel::toggleCompleted(const QModelIndex& index)
{
    coreService.toggleTaskCompletionStatus(itemAt(index.row()));
    requestDataUpdate();
}

void TaskModel::replaceItemAt(const int row, const Task& newItem)
{
    Task oldItem = itemAt(row);
    coreService.editTask(oldItem, newItem);
    requestDataUpdate();
}

bool TaskModel::moveRows(const QModelIndex& sourceParent,
                         int sourceRow,
                         int count,
                         const QModelIndex& destinationParent,
                         int destinationChild)
{
    // If item is dropped below all rows, destination child would be -1
    int destinationRow
        = (destinationChild == -1) ? rowCount() - 1 : destinationChild;

    std::vector<std::pair<std::string, int>> priorities;
    priorities.reserve(rowCount());

    // Assign priorities for tasks based on their row number,
    // then swap priorities for tasks at source and destination rows
    for (int row = 0; row < rowCount(); ++row) {
        priorities.push_back({itemAt(row).uuid(), row});
    }
    std::swap(priorities[sourceRow].second, priorities[destinationRow].second);

    coreService.registerTaskPriorities(std::move(priorities));
    requestDataUpdate();

    return true;
}
