#include <QSize>
#include "todoitemslistmodel.h"
#include "db_layer/db_helper.h"


TodoItemsListModel::TodoItemsListModel(QObject* parent) :
    QAbstractListModel(parent) 
{
    queryData();
}

void TodoItemsListModel::queryData() {
    setItems(TodoItemDataSource::getUncompleteTodoItems());
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
    updateItemsPriority(items);
    endRemoveRows();
    return true;
}

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
            return item.toString();
        case Qt::SizeHintRole:
            return QSize(10, 18); // TODO consider change through config
        case Qt::CheckStateRole:
            return item.isCompleted();
        case CopyToPomodoroRole:
            return item.tagsAndNameAsString();
        case GetIdRole:
            return item.getId();
        default:
            return QVariant();
    }
}

QHash<int, QByteArray> TodoItemsListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[CopyToPomodoroRole] = "copy";
    return roles;
}

void TodoItemsListModel::addTodoItem(TodoItem item) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    item.setId(TodoItemDataSource::storeTodoItem(item));
    items.insert(rowCount(), item);
    endInsertRows();
}

void TodoItemsListModel::incrementPomodoros(int row, int incrementBy) {
    items[row].setSpentPomodoros(items[row].getSpentPomodoros() + incrementBy);
    // TodoItemDataSource::incrementSpentPomodoros(items[row]);
    // NOTE spent_pomodoros in underlying db will be incremented by SQL trigger
}

TodoItem TodoItemsListModel::getTodoItemByModelIndex(const QModelIndex& index) {
    return items[index.row()];
}

void TodoItemsListModel::updateTodoItem(const QModelIndex& index, TodoItem updatedItem) {
    updatedItem.setId(items[index.row()].getId());
    items[index.row()] = updatedItem;
    TodoItemDataSource::updateTodoItem(updatedItem);
}

void TodoItemsListModel::removeTodoItem(const QModelIndex& index) {
    beginRemoveRows(QModelIndex(), index.row(), index.row());
    TodoItemDataSource::removeTodoItem(getTodoItemByModelIndex(index).getId());
    items.removeAt(index.row());
    endRemoveRows();
}

void TodoItemsListModel::toggleCompleted(const QModelIndex& index) {
    items[index.row()].setCompleted(!items[index.row()].isCompleted());
    TodoItemDataSource::setItemChecked(items[index.row()].getId(), items[index.row()].isCompleted());
}

void TodoItemsListModel::updateItemsPriority(const QList<TodoItem>& items) const {
    TodoItemDataSource::setItemsPriority(items);
}
