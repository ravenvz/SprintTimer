#include "todoitemmodel.h"
#include <QSize>

TodoItemModel::TodoItemModel(IPomodoroService& pomodoroService, QObject* parent)
    : pomodoroService{pomodoroService}
{
    retrieveData();
}

void TodoItemModel::retrieveData()
{
    pomodoroService.requestUnfinishedTasks(
        std::bind(&TodoItemModel::onDataChanged, this, std::placeholders::_1));
}

void TodoItemModel::onDataChanged(const std::vector<TodoItem>& tasks)
{
    beginResetModel();
    storage = tasks;
    endResetModel();
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
    case PriorityRole:
        // TODO need to figure that one out
        return 10000;
    // return columnData(index.row(), Column::Priority);
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
}

void TodoItemModel::remove(const QModelIndex& index) { remove(index.row()); }

void TodoItemModel::remove(const int row)
{
    pomodoroService.removeTask(itemAt(row));
}

TodoItem TodoItemModel::itemAt(const int row) const { return storage[row]; }

void TodoItemModel::toggleCompleted(const QModelIndex& index)
{
    pomodoroService.toggleTaskCompletionStatus(itemAt(index.row()));

    // QVariant state = data(index, Qt::CheckStateRole);
    // QVariant value = QVariant(!state.toBool());
    // bool completedToggled = QSqlTableModel::setData(
    //     index.model()->index(index.row(),
    //     static_cast<int>(Column::Completed)),
    //     value,
    //     Qt::EditRole);
    // bool timeStampUpdated = QSqlTableModel::setData(
    //     index.model()->index(index.row(),
    //                          static_cast<int>(Column::LastModified)),
    //     QVariant(QDateTime::currentDateTime()),
    //     Qt::EditRole);
    // if (!(completedToggled && timeStampUpdated)) {
    //     revertAll();
    //     return false;
    // }
    // else {
    //     submitAll();
    // }
    // return completedToggled && timeStampUpdated;
}

void TodoItemModel::replaceItemAt(const int row, const TodoItem& newItem)
{
    TodoItem oldItem = itemAt(row);
    pomodoroService.editTask(oldItem, newItem);
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
    QList<int> newPriorities;
    for (int row = 0; row < rowCount(); ++row) {
        newPriorities << row;
    }

    newPriorities.move(sourceRow, destinationRow);

    for (int i = 0; i < newPriorities.size(); ++i) {
        setItemPriority(newPriorities[i], i);
    }

    // return submitAllInTransaction();
    return false;
}

bool TodoItemModel::setItemPriority(const int row, const int priority)
{
    // return QSqlTableModel::setData(
    //     index(row, static_cast<int>(Column::Priority)), priority,
    //     Qt::EditRole);
    return false;
}
