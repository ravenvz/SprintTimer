#ifndef TODOITEMSLISTMODEL_H
#define TODOITEMSLISTMODEL_H

#include <QAbstractListModel>
#include "core/entities.h"


class TodoItemsListModel : public QAbstractListModel
{
public:
    explicit TodoItemsListModel(QObject* parent = 0);
    // virtual ~TodoItemsListModel ();
    void setItems(std::vector<TodoItem> items);
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    std::vector<TodoItem> items;
};

#endif // TODOITEMSLISTMODEL_H
