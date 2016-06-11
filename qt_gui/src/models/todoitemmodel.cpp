#include "todoitemmodel.h"
#include <QSize>
#include <algorithm>
#include <iostream>

TodoItemModel::TodoItemModel(IPomodoroService& pomodoroService, QObject* parent)
    : AsyncListModel{parent}
    , pomodoroService{pomodoroService}
{
    silentUpdate();
}

void TodoItemModel::requestDataUpdate()
{
    pomodoroService.requestUnfinishedTasks(
        std::bind(&TodoItemModel::onDataChanged, this, std::placeholders::_1));
}

void TodoItemModel::onDataChanged(const std::vector<TodoItem>& tasks)
{
    std::cout << "On data changed called" << silent << std::endl;
    beginResetModel();
    storage = tasks;
    endResetModel();
    broadcastUpdateFinished();
}

Qt::DropActions TodoItemModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Qt::DropActions TodoItemModel::supportedDragActions() const
{
    return Qt::MoveAction;
}

Qt::ItemFlags TodoItemModel::flags(const QModelIndex& index) const
{
    if (!index.isValid() || index.model() != this) {
        return Qt::ItemIsDropEnabled;
    }
    return QAbstractListModel::flags(index) | Qt::ItemIsUserCheckable
        | Qt::ItemIsDragEnabled;
}

QVariant TodoItemModel::data(const QModelIndex& index, int role) const
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

int TodoItemModel::rowCount(const QModelIndex& parent) const
{
    return static_cast<int>(storage.size());
}

void TodoItemModel::insert(const TodoItem& item)
{
    pomodoroService.registerTask(item);
    requestDataUpdate();
}

void TodoItemModel::remove(const QModelIndex& index) { remove(index.row()); }

void TodoItemModel::remove(const int row)
{
    pomodoroService.removeTask(itemAt(row));
    requestDataUpdate();
}

TodoItem TodoItemModel::itemAt(const int row) const { return storage[row]; }

void TodoItemModel::toggleCompleted(const QModelIndex& index)
{
    pomodoroService.toggleTaskCompletionStatus(itemAt(index.row()));
    requestDataUpdate();
}

void TodoItemModel::replaceItemAt(const int row, const TodoItem& newItem)
{
    TodoItem oldItem = itemAt(row);
    pomodoroService.editTask(oldItem, newItem);
    requestDataUpdate();
}

bool TodoItemModel::moveRows(const QModelIndex& sourceParent,
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
