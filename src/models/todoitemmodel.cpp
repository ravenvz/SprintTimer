#include <QSize>
#include <QSet>
#include "todoitemmodel.h"
#include "db_layer/db_service.h"

TodoItemModel::TodoItemModel(QObject* parent)
    : SqliteTableModel(parent)
{
    setTable("task_view");
    setEditStrategy(QSqlTableModel::OnManualSubmit);
    insertTodoItemQuery.prepare(
        "insert into todo_item (name, estimated_pomodoros, spent_pomodoros, "
        "completed, priority, last_modified) "
        "values (:name, :estimated_pomodoros, :spent_pomodoros, :completed, "
        ":priority, :last_modified)");
    findTagByNameQuery.prepare("select id from tag where name = (:name)");
    insertTagQuery.prepare("insert or ignore into tag (name) values (:name)");
    createRelationQuery.prepare(
        "insert into todotag (tag_id, todo_id) values (:tag_id, :todo_id);");
    removeRelationQuery.prepare(
        "delete from todotag where todo_id = (:todo_id) and "
        "tag_id in (select id from tag where name = (:tag_name))");
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
    // if (!index.isValid() || index.row() >= items.count() || index.model() !=
    // this)
    //         return Qt::ItemIsDropEnabled; // we allow drops outside the items
    if (!index.isValid() || index.model() != this) {
        return Qt::ItemIsDropEnabled;
    }
    return QSqlTableModel::flags(index) | Qt::ItemIsUserCheckable
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
    // return columnData(index.row(), Column::Tags);
    case DescriptionRole:
        return columnData(index.row(), Column::Name);
    case StatsRole:
        return QString("%1/%2")
            .arg(columnData(index.row(), Column::SpentPomodoros).toString())
            .arg(
                columnData(index.row(), Column::EstimatedPomodoros).toString());
    case Qt::CheckStateRole:
        return columnData(index.row(), Column::Completed);
    case GetSpentPomodorosRole:
        return columnData(index.row(), Column::SpentPomodoros);
    case PriorityRole:
        return columnData(index.row(), Column::Priority);
    case GetIdRole:
        return columnData(index.row(), Column::Id);
    default:
        return QSqlTableModel::data(index, role);
    }
}

bool TodoItemModel::insert(const TodoItem& item)
{
    QSqlDatabase::database().transaction();
    bool shouldRollback = false;
    const int newItemPriority = 10000;
    insertTodoItemQuery.bindValue(
        ":name", QVariant(QString::fromStdString(item.name())));
    insertTodoItemQuery.bindValue(":estimated_pomodoros",
                                  QVariant(item.estimatedPomodoros()));
    insertTodoItemQuery.bindValue(":spent_pomodoros",
                                  QVariant(item.spentPomodoros()));
    insertTodoItemQuery.bindValue(":completed", QVariant(item.isCompleted()));
    insertTodoItemQuery.bindValue(":priority", QVariant(newItemPriority));
    insertTodoItemQuery.bindValue(":last_modified",
                                  QVariant(QDateTime::currentDateTime()));
    if (!insertTodoItemQuery.exec())
        shouldRollback = true;
    QVariant todoId = insertTodoItemQuery.lastInsertId();

    QVariantList tagIds;
    // TODO should really get rid of this double-convertion here
    //
    QSet<QString> tagsToInsert;
    for (const auto& tag : item.tags()) {
        tagsToInsert << QString::fromStdString(tag);
    }
    // QSet<QString> tagsToInsert
    //     =
    //     QSet<QString>::fromList(QList<std::string>::fromStdList(item.tags()));
    if (!insertTags(todoId, tagsToInsert))
        shouldRollback = true;

    if (shouldRollback || !QSqlDatabase::database().commit()) {
        QSqlDatabase::database().rollback();
        return false;
    }
    select();
    return true;
}

bool TodoItemModel::remove(const QModelIndex& index)
{
    return remove(index.row());
}

bool TodoItemModel::remove(const int row)
{
    return QSqlTableModel::removeRow(row) && submitAll() && select();
}

TodoItem TodoItemModel::itemAt(const int row) const
{
    QSqlRecord rowRecord{QSqlTableModel::record(row)};
    QString name{columnData(rowRecord, Column::Name).toString()};
    int estimatedPomodoros{
        columnData(rowRecord, Column::EstimatedPomodoros).toInt()};
    int spentPomodoros{columnData(rowRecord, Column::SpentPomodoros).toInt()};
    QVariant tagsVariant{columnData(rowRecord, Column::Tags)};
    QStringList qTags = tagsVariant.isNull()
        ? QStringList()
        : tagsVariant.toString().split(",");
    std::list<std::string> tags;
    std::transform(qTags.cbegin(),
                   qTags.cend(),
                   std::back_inserter(tags),
                   [](const auto& tag) { return tag.toStdString(); });
    bool completed{columnData(rowRecord, Column::Completed).toBool()};
    return TodoItem{name.toStdString(),
                    estimatedPomodoros,
                    spentPomodoros,
                    tags,
                    completed};
}

bool TodoItemModel::toggleCompleted(const QModelIndex& index)
{
    QVariant state = data(index, Qt::CheckStateRole);
    QVariant value = QVariant(!state.toBool());
    bool completedToggled = QSqlTableModel::setData(
        index.model()->index(index.row(), static_cast<int>(Column::Completed)),
        value,
        Qt::EditRole);
    bool timeStampUpdated = QSqlTableModel::setData(
        index.model()->index(index.row(),
                             static_cast<int>(Column::LastModified)),
        QVariant(QDateTime::currentDateTime()),
        Qt::EditRole);
    if (!(completedToggled && timeStampUpdated)) {
        revertAll();
        return false;
    }
    else {
        submitAll();
    }
    return completedToggled && timeStampUpdated;
}

QVector<TodoItem> TodoItemModel::items()
{
    auto numItems = numRecords();
    QVector<TodoItem> allItems;
    allItems.reserve(numItems);
    for (int row = 0; row < numItems; ++row) {
        allItems.push_back(itemAt(row));
    }
    return allItems;
}

QVector<TodoItemModel::TodoItemWithTimeStamp>
TodoItemModel::itemsWithTimestamp()
{
    auto numItems = numRecords();
    QVector<TodoItemWithTimeStamp> allItems;
    allItems.reserve(numItems);
    for (int row = 0; row < numItems; ++row) {
        allItems.push_back(std::make_pair(
            columnData(row, Column::LastModified).toDateTime().date(),
            QString::fromStdString(itemAt(row).toString())));
    }
    return allItems;
}

bool TodoItemModel::replaceItemAt(const int row, const TodoItem& newItem)
{
    TodoItem oldItem = itemAt(row);
    QSet<QString> oldTags;
    QSet<QString> newTags;
    for (const auto& tag : oldItem.tags()) {
        oldTags << QString::fromStdString(tag);
    }
    for (const auto& tag : newItem.tags()) {
        newTags << QString::fromStdString(tag);
    }
    // QSet<QString> oldTags = QSet<QString>::fromList(oldItem.tags());
    // QSet<QString> newTags = QSet<QString>::fromList(newItem.tags());
    if (oldTags != newTags) {
        QSet<QString> tagsToRemove = oldTags;
        QSet<QString> tagsToInsert = newTags;
        tagsToRemove.subtract(newTags);
        tagsToInsert.subtract(oldTags);
        if (!updateTags(row, tagsToRemove, tagsToInsert))
            return false;
    }
    return updateRow(row, newItem);
}

void TodoItemModel::setNotCompletedFilter()
{
    setFilter("completed = 0 or last_modified > datetime('now', '-1 day')");
}

void TodoItemModel::setCompletedInIntervalFilter(const DateInterval& interval)
{
    QString filter{QString("completed = 1 and date(last_modified) >= '%1' and "
                           "date(last_modified) <= '%2'")
                       .arg(interval.startDate.toString("yyyy-MM-dd"))
                       .arg(interval.endDate.toString("yyyy-MM-dd"))};
    setFilter(filter);
}

long long TodoItemModel::itemIdAt(const int row) const
{
    return columnData(row, Column::Id).toInt();
}

QString TodoItemModel::itemNameAt(const int row) const
{
    return columnData(row, Column::Name).toString();
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

    return submitAllInTransaction();
}

bool TodoItemModel::setItemPriority(const int row, const int priority)
{
    return QSqlTableModel::setData(
        index(row, static_cast<int>(Column::Priority)), priority, Qt::EditRole);
}

QVariant TodoItemModel::columnData(const QSqlRecord& rowRecord,
                                   const Column& column) const
{
    return rowRecord.value(static_cast<int>(column));
}

QVariant TodoItemModel::columnData(const int row, const Column& column) const
{
    return record(row).value(static_cast<int>(column));
}

bool TodoItemModel::updateTags(const int row,
                               const QSet<QString>& tagsToRemove,
                               const QSet<QString>& tagsToInsert)
{
    QVariant todoId = data(index(row, static_cast<int>(Column::Id)), GetIdRole);
    QSqlDatabase::database().transaction();
    bool shouldRollback = !(insertTags(todoId, tagsToInsert)
                            && removeTags(todoId, tagsToRemove));
    if (shouldRollback || !QSqlDatabase::database().commit()) {
        QSqlDatabase::database().rollback();
        return false;
    }
    return select();
}

bool TodoItemModel::insertTags(QVariant itemId, const QSet<QString>& tags)
{
    bool success = true;
    for (const auto& tag : tags) {
        findTagByNameQuery.bindValue(":name", tag);
        findTagByNameQuery.exec();
        QVariant tagId;
        if (findTagByNameQuery.next()) {
            tagId = findTagByNameQuery.value(0);
        }
        else {
            insertTagQuery.bindValue(":name", tag);
            if (!insertTagQuery.exec())
                success = false;
            tagId = insertTagQuery.lastInsertId();
        }
        createRelationQuery.bindValue(":tag_id", tagId);
        createRelationQuery.bindValue(":todo_id", itemId);
        if (!createRelationQuery.exec())
            success = false;
    }
    return success;
}

bool TodoItemModel::removeTags(QVariant itemId, const QSet<QString>& tags)
{
    // Note that after relation removed, tag would be removed by trigger
    // if it is orphaned (has no assotiated todo items)
    bool success = true;
    for (const auto& tag : tags) {
        removeRelationQuery.bindValue(":todo_id", itemId);
        removeRelationQuery.bindValue(":tag_name", tag);
        if (!removeRelationQuery.exec())
            success = false;
    }
    return success;
}

bool TodoItemModel::updateRow(const int row, const TodoItem& newItem)
{
    bool nameUpdated
        = QSqlTableModel::setData(index(row, static_cast<int>(Column::Name)),
                                  QString::fromStdString(newItem.name()),
                                  Qt::EditRole);
    bool estimatedUpdated = QSqlTableModel::setData(
        index(row, static_cast<int>(Column::EstimatedPomodoros)),
        newItem.estimatedPomodoros(),
        Qt::EditRole);
    bool spentUpdated = QSqlTableModel::setData(
        index(row, static_cast<int>(Column::SpentPomodoros)),
        newItem.spentPomodoros(),
        Qt::EditRole);
    bool completedUpdated = QSqlTableModel::setData(
        index(row, static_cast<int>(Column::Completed)),
        newItem.isCompleted(),
        Qt::EditRole);
    bool timestampUpdated = QSqlTableModel::setData(
        index(row, static_cast<int>(Column::LastModified)),
        QDateTime::currentDateTime(),
        Qt::EditRole);
    if (nameUpdated && estimatedUpdated && spentUpdated && completedUpdated
        && timestampUpdated) {
        submitAll();
        return true;
    };
    revertAll();
    return false;
}
