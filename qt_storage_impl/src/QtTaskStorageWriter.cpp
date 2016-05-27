#include "qt_storage_impl/QtTaskStorageWriter.h"


QtTaskStorageWriter::QtTaskStorageWriter(DBService& dbService)
    : dbService{dbService}
{
    addQueryId = dbService.prepare(
        "BEGIN TRANSACTION; "
        "INSERT INTO todo_item (name, estimated_pomodoros, spent_pomodoros, "
        "completed, priority, last_modified, uuid) "
        "VALUES (:name, :estimated_pomodoros, :spent_pomodoros, :completed, "
        ":priority, :last_modified, :uuid); "
        "INSERT INTO todotag (tag_id, todo_id) VALUES (:tag_id, :todo_id); "
        "COMMIT;");
    removeQueryId = dbService.prepare("");
    editQueryId = dbService.prepare("");
    incrementSpentQueryId = dbService.prepare(
        "update todo_item set spent_pomodoros = spent_pomodoros + 1 "
        "where todo_item.uuid = (:todo_uuid);");
    decrementSpentQueryId = dbService.prepare(
        "update todo_item set spent_pomodoros = spent_pomodoros - 1 "
        "where todo_item.uuid = (:todo_uuid);");
}

void QtTaskStorageWriter::save(const TodoItem& task) {}

void QtTaskStorageWriter::remove(const TodoItem& task) {}

void QtTaskStorageWriter::edit(const TodoItem& task, const TodoItem& editedTask)
{
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
