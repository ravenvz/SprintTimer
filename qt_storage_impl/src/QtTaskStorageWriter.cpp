#include "qt_storage_impl/QtTaskStorageWriter.h"
#include "utils/DateTimeConverter.h"
#include "qt_storage_impl/utils.h"
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
    toggleCompletionQueryId
        = dbService.prepare("update todo_item "
                            "set completed = not completed, "
                            "last_modified = :time_stamp "
                            "where uuid = :uuid;");
    updatePrioritiesQueryId = dbService.prepare(
        "update todo_item set priority = :priority where uuid = :uuid;");
    editTagQueryId = dbService.prepare(
        "update tag set name = :new_name where name = :old_name;");
}

void QtTaskStorageWriter::save(const Task& task)
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
                        DateTimeConverter::qDateTime(task.lastModified()));
    dbService.bindValue(addTaskQueryId, ":uuid", uuid);

    dbService.transaction();

    dbService.executePrepared(addTaskQueryId);
    insertTags(uuid, task.tags());

    dbService.commit();
}

void QtTaskStorageWriter::insertTags(const QString& taskUuid,
                                     const std::list<Tag>& tags)
{
    for (const auto& tag : tags) {
        dbService.bindValue(
            insertTagQueryId, ":tag", QString::fromStdString(tag.name()));
        dbService.bindValue(insertTagQueryId, ":uuid", taskUuid);
        dbService.executePrepared(insertTagQueryId);
    }
}

void QtTaskStorageWriter::removeTags(const QString& taskUuid,
                                     const std::list<Tag>& tags)
{
    for (const auto& tag : tags) {
        dbService.bindValue(removeTagQueryId, ":uuid", taskUuid);
        dbService.bindValue(
            removeTagQueryId, ":tag", QString::fromStdString(tag.name()));
        dbService.executePrepared(removeTagQueryId);
    }
}

void QtTaskStorageWriter::remove(const Task& task)
{
    // Note that tags are removed by trigger
    dbService.bindValue(
        removeTaskQueryId, ":uuid", QString::fromStdString(task.uuid()));
    dbService.executePrepared(removeTaskQueryId);
}

void QtTaskStorageWriter::edit(const Task& task, const Task& editedTask)
{
    const QString taskUuid = QString::fromStdString(task.uuid());

    dbService.bindValue(
        editQueryId, ":name", QString::fromStdString(editedTask.name()));
    dbService.bindValue(
        editQueryId, ":estimated_pomodoros", editedTask.estimatedPomodoros());
    dbService.bindValue(
        editQueryId,
        ":last_modified",
        DateTimeConverter::qDateTime(editedTask.lastModified()));
    dbService.bindValue(editQueryId, ":uuid", taskUuid);

    auto oldTags = task.tags();
    auto newTags = editedTask.tags();
    oldTags.sort();
    newTags.sort();
    std::list<Tag> tagsToRemove;
    std::list<Tag> tagsToInsert;

    twoWayDiff(cbegin(oldTags),
               cend(oldTags),
               cbegin(newTags),
               cend(newTags),
               std::back_inserter(tagsToRemove),
               std::back_inserter(tagsToInsert));

    dbService.transaction();

    dbService.executePrepared(editQueryId);
    removeTags(taskUuid, tagsToRemove);
    insertTags(taskUuid, tagsToInsert);

    dbService.commit();
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

void QtTaskStorageWriter::toggleTaskCompletionStatus(const std::string& uuid,
                                                     const DateTime& timeStamp)
{
    dbService.bindValue(
        toggleCompletionQueryId, ":uuid", QString::fromStdString(uuid));
    dbService.bindValue(toggleCompletionQueryId,
                        ":time_stamp",
                        DateTimeConverter::qDateTime(timeStamp));
    dbService.executePrepared(toggleCompletionQueryId);
}

void QtTaskStorageWriter::updatePriorities(
    std::vector<std::pair<std::string, int>>&& priorities)
{
    dbService.requestTransaction();

    for (const auto& pair : priorities) {
        dbService.bindValue(updatePrioritiesQueryId,
                            ":uuid",
                            QString::fromStdString(pair.first));
        dbService.bindValue(updatePrioritiesQueryId, ":priority", pair.second);
        dbService.executePrepared(updatePrioritiesQueryId);
    }

    dbService.commit();
}

void QtTaskStorageWriter::editTag(const std::string& oldName,
                                  const std::string& newName)
{
    dbService.bindValue(
        editTagQueryId, ":old_name", QString::fromStdString(oldName));
    dbService.bindValue(
        editTagQueryId, ":new_name", QString::fromStdString(newName));
    dbService.executePrepared(editTagQueryId);
}
