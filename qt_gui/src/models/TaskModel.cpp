/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#include <QSize>
#include <core/use_cases/RequestUnfinishedTasks.h>
#include <core/utils/Algutils.h>
#include <string>
#include <vector>

namespace sprint_timer::ui::qt_gui {

using entities::Task;
using namespace use_cases;

TaskModel::TaskModel(ICoreService& coreService,
                     ITaskStorageReader& taskStorageReader,
                     QueryExecutor& queryExecutor,
                     QObject* parent)
    : AsyncListModel{parent}
    , coreService{coreService}
    , taskStorageReader{taskStorageReader}
    , queryExecutor{queryExecutor}
{
    synchronize();
}

void TaskModel::requestDataUpdate()
{

    queryExecutor.executeQuery(std::make_unique<RequestUnfinishedTasks>(
        taskStorageReader,
        [this](const auto& tasks) { onDataChanged(tasks); }));
    // coreService.requestUnfinishedTasks(
    //     [this](const auto& tasks) { this->onDataChanged(tasks); });
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

    const Task& taskRef = storage[static_cast<size_t>(index.row())];

    switch (role) {
    case Qt::DisplayRole:
        return QString{"%1 %2 %3/%4"}
            .arg(QString::fromStdString(prefixTags(taskRef.tags())))
            .arg(QString::fromStdString(taskRef.name()))
            .arg(taskRef.actualCost())
            .arg(taskRef.estimatedCost());
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

void TaskModel::remove(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    coreService.removeTask(itemAt(row));
    storage.erase(storage.begin() + row);
    // TODO
    // As a workaround, data update is delayed, because delete operation
    // takes quite a long time (time is needed to collect sprints) and
    // in current implementation we have no way to know if command is
    // completed or not. This should be replaced when more flexible
    // approach is introduced.
    startTimer(std::chrono::seconds{1});
    endRemoveRows();
}

Task TaskModel::itemAt(int row) const
{
    return storage.at(static_cast<size_t>(row));
}

void TaskModel::toggleCompleted(const QModelIndex& index)
{
    coreService.toggleTaskCompletionStatus(itemAt(index.row()));
    requestDataUpdate();
}

void TaskModel::replaceItemAt(int row, const Task& newItem)
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
    // TODO Sadly this leads to ignoring dropping item below the last item,
    // should study Qt documentation for item reordering
    if (destinationChild == -1)
        return false;

    // This offset is needed because slide takes iterator that should point
    // before desired location as position argument
    if (sourceRow < destinationChild)
        ++destinationChild;

    auto uuidsInOrder = [& st = storage]() {
        std::vector<std::string> uuids;
        uuids.reserve(st.size());
        std::transform(st.cbegin(),
                       st.cend(),
                       std::back_inserter(uuids),
                       [](const auto& task) { return task.uuid(); });
        return uuids;
    };

    auto old_order = uuidsInOrder();

    beginResetModel();
    utils::slide(storage.begin() + sourceRow,
                 storage.begin() + sourceRow + 1,
                 storage.begin() + destinationChild);
    endResetModel();

    auto new_order = uuidsInOrder();

    coreService.registerTaskPriorities(std::move(old_order),
                                       std::move(new_order));

    return true;
}

void TaskModel::timerEvent(QTimerEvent* event) { requestDataUpdate(); }

} // namespace sprint_timer::ui::qt_gui
