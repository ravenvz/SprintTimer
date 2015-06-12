#ifndef TODOITEMSLISTMODEL_H
#define TODOITEMSLISTMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include "core/entities.h"


class TodoItemsListModel : public QAbstractListModel
{
public:
    explicit TodoItemsListModel(QObject* parent = 0);
    // virtual ~TodoItemsListModel ();
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QHash<int, QByteArray> roleNames() const override;
    enum customRoles { CopyToPomodoroRole = Qt::UserRole + 1 };
    void addTodoItem(TodoItem item);
    void incrementPomodoros(int row, int incrementBy);
    void removeTodoItem(const QModelIndex& index);
    TodoItem getTodoItemByModelIndex(const QModelIndex& index);
    void updateTodoItem(const QModelIndex& index, const TodoItem updatedItem);

private:
    QList<TodoItem> items;
    void setItems(QList<TodoItem> items);
};

#endif // TODOITEMSLISTMODEL_H
