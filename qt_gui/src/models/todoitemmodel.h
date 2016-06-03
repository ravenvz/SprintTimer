#ifndef TODOITEMSLISTMODEL_H
#define TODOITEMSLISTMODEL_H

#include "core/IPomodoroService.h"
#include "core/TimeSpan.h"
#include "core/entities/TodoItem.h"
#include "utils/DateInterval.h"
#include <QAbstractListModel>

class TodoItemModel : public QAbstractListModel {
public:
    using TodoItemWithTimeStamp = std::pair<QDate, QString>;

    explicit TodoItemModel(IPomodoroService& pomodoroService,
                           QObject* parent = 0);

    void retrieveData();

    // Override to support drag and drop.
    Qt::DropActions supportedDropActions() const override;

    // Override to support drag and drop.
    Qt::DropActions supportedDragActions() const override;

    // Override to support drag and drop.
    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Override to provide support for custom roles as well as to customize
    // behaviour
    // of the default roles.
    QVariant data(const QModelIndex& index,
                  int role = Qt::DisplayRole) const override;

    // Override to support drag and drop. Changes items' priorities instead of
    // removing row and inserting it at destination position as in default
    // behavour for drag and drop. That default behaviour would not work here,
    // as sqlite view is set to this model, and removing row from it would have
    // undesired consequences.
    bool moveRows(const QModelIndex& sourceParent,
                  int sourceRow,
                  int count,
                  const QModelIndex& destinationParent,
                  int destinationChild) override;

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    enum customRoles {
        TagsRole = Qt::UserRole + 1,
        DescriptionRole,
        StatsRole,
        GetIdRole,
        GetSpentPomodorosRole,
        PriorityRole
    };

    // Insert new TodoItem into the database.
    // Return boolean, indicating success of the operation.
    // Changes are rolled back in case of failure.
    void insert(const TodoItem& item);

    // Remove item with given index and return boolean, indicating success of
    // the operation.
    void remove(const QModelIndex& index);

    // Overload that accepts row number as item identifier.
    void remove(const int row);

    // Return item at given row. This is a convinient method that allows to get
    // item
    // without verbose calls to data().
    TodoItem itemAt(const int row) const;

    // Mark item as completed if it is not completed and vice versa.
    void toggleCompleted(const QModelIndex& index);

    // Replace data of item at given row with data from the newItem.
    void replaceItemAt(const int row, const TodoItem& newItem);

private:
    IPomodoroService& pomodoroService;
    std::vector<TodoItem> storage;
    // Sql helper queries that are needed to maintain database invariants.
    enum class Column {
        Id,
        Name,
        EstimatedPomodoros,
        SpentPomodoros,
        Priority,
        Completed,
        Tags,
        LastModified,
        Uuid
    };

    void onDataChanged(const std::vector<TodoItem>& tasks);
};

#endif // TODOITEMSLISTMODEL_H
