#ifndef TODOITEMSLISTMODEL_H
#define TODOITEMSLISTMODEL_H

#include <QSqlTableModel>
#include "sqlitetablemodel.h"
#include <QHash>
#include <QSqlQuery>
#include "core/entities/TodoItem.h"
#include "core/timeinterval.h"
#include "utils/DateInterval.h"

class TodoItemModel : public SqliteTableModel {
public:
    using TodoItemWithTimeStamp = std::pair<QDate, QString>;

    explicit TodoItemModel(QObject* parent = 0);

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
    bool insert(const TodoItem& item);

    // Remove item with given index and return boolean, indicating success of
    // the operation.
    bool remove(const QModelIndex& index);

    // Overload that accepts row number as item identifier.
    bool remove(const int row);

    // Return item at given row. This is a convinient method that allows to get
    // item
    // without verbose calls to data().
    TodoItem itemAt(const int row) const;

    // Mark item as completed if it is not completed and vice versa.
    bool toggleCompleted(const QModelIndex& index);

    // Return QVector of all items (not only cached) in the model.
    QVector<TodoItem> items();

    // Return vector of pairs of date and time when item was last edited and
    // item itself.
    QVector<TodoItemModel::TodoItemWithTimeStamp> itemsWithTimestamp();

    // Replace data of item at given row with data from the newItem.
    bool replaceItemAt(const int row, const TodoItem& newItem);

    // Set filter to show only items that are not completed or last updated
    // less than 1 day ago.
    void setNotCompletedFilter();

    // Set filter to show only items that are completed between dates in the
    // given interval.
    void setCompletedInIntervalFilter(const DateInterval& interval);

    // Return item id at a given row.
    long long itemIdAt(const int row) const;

    // Return item name at a given row.
    QString itemNameAt(const int row) const;

private:
    // Sql helper queries that are needed to maintain database invariants.
    QSqlQuery insertTodoItemQuery;
    QSqlQuery findTagByNameQuery;
    QSqlQuery insertTagQuery;
    QSqlQuery createRelationQuery;
    QSqlQuery removeRelationQuery;

    enum class Column {
        Id,
        Name,
        EstimatedPomodoros,
        SpentPomodoros,
        Priority,
        Completed,
        Tags,
        LastModified
    };

    // Return value in the field identified by QSqlRecord and column number
    // wrapped in QVariant
    QVariant columnData(const QSqlRecord& rowRecord,
                        const Column& column) const;

    // Return value in given row and column wrapped in QVariant.
    QVariant columnData(const int row, const Column& column) const;

    // Given row number and two sets of tag names, update tags accordingly for
    // item at this row.
    // Tag sets may contain identical tags.
    // Relations between items and tags are also updated.
    bool updateTags(const int row,
                    const QSet<QString>& tagsToRemove,
                    const QSet<QString>& tagsToInsert);

    // Given row number and set of tag names, insert tags in the database.
    // Create relations between items and tags.
    bool insertTags(QVariant itemId, const QSet<QString>& tags);

    // Given row number and set of tag names, remove tags from the database.
    // Remove relations between items and tags.
    // Note that after removing relation, tag might become orphaned (that is,
    // not
    // associated with any item). This implementation relies on the SQL trigger
    // to
    // clean up such orphaned tags.
    bool removeTags(QVariant itemId, const QSet<QString>& tags);

    // Replace item data at given row with data from the newItem.
    // Note that this method does not update tags.
    bool updateRow(const int row, const TodoItem& newItem);

    // Set priority of item at given row.
    bool setItemPriority(const int row, const int priority);
};

#endif // TODOITEMSLISTMODEL_H
