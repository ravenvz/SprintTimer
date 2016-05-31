#include "qt_storage_impl/QtTaskStorageWriter.h"
#include <algorithm>


QtTaskStorageWriter::QtTaskStorageWriter(DBService& dbService)
    : dbService{dbService}
{
    addTaskQueryId = dbService.prepare(
        "INSERT INTO todo_item (name, estimated_pomodoros, spent_pomodoros, "
        "completed, priority, last_modified, uuid) "
        "VALUES (:name, :estimated_pomodoros, :spent_pomodoros, :completed, "
        ":priority, :last_modified, :uuid); ");
    insertTagQueryId = dbService.prepare(
        "insert into task_tag_view(tagname, uuid) values(:tag, :uuid);");
    removeTagQueryId = dbService.prepare(
        "delete from todotag where todo_uuid = :uuid "
        "and tag_id in (select id from tag where name = :tag);");
    removeTaskQueryId
        = dbService.prepare("delete from todo_item where uuid = (:uuid)");
    editQueryId
        = dbService.prepare("update todo_item set name = :name, "
                            "estimated_pomodoros = :estimated_pomodoros, "
                            "last_modified = :last_modified "
                            "where uuid = :uuid;");
    incrementSpentQueryId = dbService.prepare(
        "update todo_item set spent_pomodoros = spent_pomodoros + 1 "
        "where todo_item.uuid = (:todo_uuid);");
    decrementSpentQueryId = dbService.prepare(
        "update todo_item set spent_pomodoros = spent_pomodoros - 1 "
        "where todo_item.uuid = (:todo_uuid);");
}

void QtTaskStorageWriter::save(const TodoItem& task)
{
    QString uuid = QString::fromStdString(task.uuid());
    dbService.bindValue(
        addTaskQueryId, ":name", QString::fromStdString(task.name()));
    dbService.bindValue(
        addTaskQueryId, ":estimated_pomodoros", task.estimatedPomodoros());
    dbService.bindValue(
        addTaskQueryId, ":spent_pomodoros", task.spentPomodoros());
    dbService.bindValue(addTaskQueryId, ":completed", task.isCompleted());
    dbService.bindValue(addTaskQueryId, ":priority", 10000);
    dbService.bindValue(addTaskQueryId,
                        ":last_modified",
                        QDateTime::fromTime_t(static_cast<unsigned>(
                            task.lastModified().toTime_t())));
    dbService.bindValue(addTaskQueryId, ":uuid", uuid);
    dbService.executePrepared(addTaskQueryId);

    insertTags(uuid, task.tags());
}

void QtTaskStorageWriter::insertTags(const QString& taskUuid,
                                     const std::list<std::string>& tags)
{
    for (const auto& tag : tags) {
        dbService.bindValue(
            insertTagQueryId, ":tag", QString::fromStdString(tag));
        dbService.bindValue(insertTagQueryId, ":uuid", taskUuid);
        dbService.executePrepared(insertTagQueryId);
    }
}

void QtTaskStorageWriter::removeTags(const QString& taskUuid,
                                     const std::list<std::string>& tags)
{
    for (const auto& tag : tags) {
        dbService.bindValue(removeTagQueryId, ":uuid", taskUuid);
        dbService.bindValue(
            removeTagQueryId, ":tag", QString::fromStdString(tag));
        dbService.executePrepared(removeTagQueryId);
    }
}

void QtTaskStorageWriter::remove(const TodoItem& task)
{
    // Note that tags are removed by trigger
    dbService.bindValue(
        removeTaskQueryId, ":uuid", QString::fromStdString(task.uuid()));
    dbService.executePrepared(removeTaskQueryId);
}

void QtTaskStorageWriter::edit(const TodoItem& task, const TodoItem& editedTask)
{
    const QString taskUuid = QString::fromStdString(task.uuid());

    dbService.bindValue(
        editQueryId, ":name", QString::fromStdString(editedTask.name()));
    dbService.bindValue(
        editQueryId, ":estimated_pomodoros", editedTask.estimatedPomodoros());
    dbService.bindValue(
        editQueryId, ":last_modified", QDateTime::currentDateTime());
    dbService.bindValue(editQueryId,
                        ":last_modified",
                        QDateTime::fromTime_t(static_cast<unsigned>(
                            editedTask.lastModified().toTime_t())));
    dbService.executePrepared(editQueryId);

    std::list<std::string> oldTags = task.tags();
    std::list<std::string> newTags = editedTask.tags();
    oldTags.sort();
    newTags.sort();
    std::list<std::string> tagsToRemove;
    std::list<std::string> tagsToInsert;

    twoWayDiff(cbegin(oldTags),
               cend(oldTags),
               cbegin(newTags),
               cend(newTags),
               std::back_inserter(tagsToRemove),
               std::back_inserter(tagsToInsert));

    std::cout << "Tags to insert:" << std::endl;
    std::copy(tagsToInsert.cbegin(),
              tagsToInsert.cend(),
              std::ostream_iterator<std::string>(std::cout, ", "));
    std::cout << std::endl;

    std::cout << "Tags to remove:" << std::endl;
    std::copy(tagsToRemove.cbegin(),
              tagsToRemove.cend(),
              std::ostream_iterator<std::string>(std::cout, ", "));
    std::cout << std::endl;

    removeTags(taskUuid, tagsToRemove);
    insertTags(taskUuid, tagsToInsert);
}

void QtTaskStorageWriter::incrementSpentPomodoros(const std::string& uuid)
{
    dbService.bindValue(
        incrementSpentQueryId, ":todo_uuid", QString::fromStdString(uuid));
    dbService.executePrepared(incrementSpentQueryId);
}

void QtTaskStorageWriter::decrementSpentPomodoros(const std::string& uuid)
{
    dbService.bindValue(
        decrementSpentQueryId, ":todo_uuid", QString::fromStdString(uuid));
    dbService.executePrepared(decrementSpentQueryId);
}
