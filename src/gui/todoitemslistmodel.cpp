#include <QSize>
#include "todoitemslistmodel.h"
#include "data_base/db_helper.h"


TodoItemsListModel::TodoItemsListModel(QObject* parent) :
    QAbstractListModel(parent) 
{
    setItems(TodoItemGateway::getUncompleteTodoItems());
}

Qt::DropActions TodoItemsListModel::supportedDropActions() const {
    return Qt::MoveAction;
}

Qt::DropActions TodoItemsListModel::supportedDragActions() const {
    return Qt::MoveAction;
}

Qt::ItemFlags TodoItemsListModel::flags(const QModelIndex &index) const {
    if (!index.isValid() || index.row() >= items.count() || index.model() != this)
            return Qt::ItemIsDropEnabled; // we allow drops outside the items
    return QAbstractListModel::flags(index) | Qt::ItemIsUserCheckable | Qt::ItemIsDragEnabled;
}

bool TodoItemsListModel::insertRows(int row, int count, const QModelIndex &parent) {
    if (count < 1 || row < 0 || row > rowCount() || parent.isValid()) {
        return false;
    }
    beginInsertRows(QModelIndex(), row, row + count - 1);
    tmp = row;
    items.insert(row, items[0]);
    endInsertRows();
    return true;
}

bool TodoItemsListModel::removeRows(int row, int count, const QModelIndex &parent) {
    if (count < 1 || row < 0 || (row + count) > rowCount() || parent.isValid()) {
        return false;
    }
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    items.swap(tmp, row);
    items.removeAt(row);
    endRemoveRows();
    return true;
}

// bool TodoItemsListModel::removeRows(int row, int count, const QModelIndex& parent) {
//     qDebug() << "Ololol";
//     // beginRemoveRows(QModelIndex(), index.row(), index.row());
//     // TodoItemGateway::removeTodoItem(getTodoItemByModelIndex(index).id);
//     // items.removeAt(index.row());
//     if (count < 1 || row < 0 || row > rowCount() || parent.isValid()) {
//         return false;
//     }
//     removeTodoItem(parent);
//     return true;
// }

void TodoItemsListModel::setItems(QList<TodoItem> items) {
    this->items = items;
}

int TodoItemsListModel::rowCount(const QModelIndex& parent) const {
    return items.size();
}

QVariant TodoItemsListModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    TodoItem item = items[index.row()];

    switch (role) {
        case Qt::DisplayRole:
            return item.asString();
        case Qt::SizeHintRole:
            return QSize(10, 18); // TODO consider change through config
        // case Qt::EditRole:
        //     return item.completed;
        case Qt::CheckStateRole:
            return item.completed;
        case CopyToPomodoroRole:
            return item.tagsAndNameAsString();
        default:
            return QVariant();
    }
}

// bool TodoItemsListModel::setData(const QModelIndex& index, const QVariant& value, int role) {
//     if (!index.isValid()) {
//         return false;
//     }
//     switch (role) {
//         case ToggleCompletedRole:
//             items[index.row()].completed = !items[index.row()].completed;
//             TodoItemGateway::setItemChecked(items[index.row()].id, items[index.row()].completed);
//             return true;
//        default:
//             return false;
//     }
// }

QHash<int, QByteArray> TodoItemsListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[CopyToPomodoroRole] = "copy";
    return roles;
}

void TodoItemsListModel::addTodoItem(TodoItem item) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    int id = TodoItemGateway::storeTodoItem(item);
    item.id = id;
    items.insert(rowCount(), item);
    endInsertRows();
}

void TodoItemsListModel::incrementPomodoros(int row, int incrementBy) {
    items[row].spentPomodoros += incrementBy;
    TodoItemGateway::incrementSpentPomodoros(items[row]);
}

TodoItem TodoItemsListModel::getTodoItemByModelIndex(const QModelIndex& index) {
    return items[index.row()];
}

void TodoItemsListModel::updateTodoItem(const QModelIndex& index, TodoItem updatedItem) {
    updatedItem.id = items[index.row()].id;
    items[index.row()] = updatedItem;
    TodoItemGateway::updateTodoItem(updatedItem);
}

void TodoItemsListModel::removeTodoItem(const QModelIndex& index) {
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    TodoItemGateway::removeTodoItem(getTodoItemByModelIndex(index).id);
    items.removeAt(index.row());
    endRemoveRows();
}

void TodoItemsListModel::toggleCompleted(const QModelIndex& index) {
    items[index.row()].completed = !items[index.row()].completed;
    TodoItemGateway::setItemChecked(items[index.row()].id, items[index.row()].completed);
}
