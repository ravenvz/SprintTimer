#include "todoitemmodel.h"
#include <QSize>
#include <algorithm>
#include <iostream>

TaskModel::TaskModel(IPomodoroService& pomodoroService, QObject* parent)
    : AsyncListModel{parent}
    , pomodoroService{pomodoroService}
{
    synchronize();
}

void TaskModel::requestDataUpdate()
{
    pomodoroService.requestUnfinishedTasks(
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
    switch (role) {
    case Qt::DisplayRole:
        return QString::fromStdString(itemAt(index.row()).toString());
    case TagsRole:
        return QString::fromStdString(itemAt(index.row()).tagsAsString());
    case DescriptionRole:
        return QString::fromStdString(storage[index.row()].name());
    case StatsRole:
        return QString("%1/%2")
            .arg(storage[index.row()].spentPomodoros())
            .arg(storage[index.row()].estimatedPomodoros());
    case Qt::CheckStateRole:
        return storage[index.row()].isCompleted();
    case GetSpentPomodorosRole:
        return storage[index.row()].spentPomodoros();
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
    pomodoroService.registerTask(item);
    requestDataUpdate();
}

void TaskModel::remove(const QModelIndex& index) { remove(index.row()); }

void TaskModel::remove(const int row)
{
    pomodoroService.removeTask(itemAt(row));
    requestDataUpdate();
}

Task TaskModel::itemAt(const int row) const { return storage.at(row); }

void TaskModel::toggleCompleted(const QModelIndex& index)
{
    pomodoroService.toggleTaskCompletionStatus(itemAt(index.row()));
    requestDataUpdate();
}

void TaskModel::replaceItemAt(const int row, const Task& newItem)
{
    Task oldItem = itemAt(row);
    pomodoroService.editTask(oldItem, newItem);
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

    pomodoroService.registerTaskPriorities(std::move(priorities));
    requestDataUpdate();

    return true;
}
