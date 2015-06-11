#include <QSize>
#include "todoitemslistmodel.h"
#include "data_base/db_helper.h"


TodoItemsListModel::TodoItemsListModel(QObject* parent) :
    QAbstractListModel(parent) 
{
    setItems(TodoItemGateway::getUncompleteTodoItems());
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
            return item.asString();
        case Qt::SizeHintRole:
            return QSize(10, 18); // TODO consider change through config
        case Qt::EditRole:
            return item.name;
        case Qt::CheckStateRole:
            return item.completed;
        case CopyToPomodoroRole:
            return item.tagsAndNameAsString();
        default:
            return QVariant();
    }
}

bool TodoItemsListModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid()) {
        return false;
    }
    switch (role) {
        case Qt::CheckStateRole:
            items[index.row()].completed = !items[index.row()].completed;
            return true;
       default:
            return false;
    }
}

Qt::ItemFlags TodoItemsListModel::flags(const QModelIndex& index) const {
    return Qt::ItemIsEditable |
           Qt::ItemIsEnabled |
           Qt::ItemIsUserCheckable |
           Qt::ItemIsSelectable |
           Qt::ItemIsDragEnabled;
}

QHash<int, QByteArray> TodoItemsListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[CopyToPomodoroRole] = "copy";
    return roles;
}

void TodoItemsListModel::addTodoItem(TodoItem item) {
    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    TodoItemGateway::storeTodoItem(item);
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

void TodoItemsListModel::updateTodoItem(const QModelIndex& index, const TodoItem updatedItem) {
    QString oldItemName = item[index.row()].name;
    item[index.row()] = updatedItem;
    // TodoItemGateway::
}
