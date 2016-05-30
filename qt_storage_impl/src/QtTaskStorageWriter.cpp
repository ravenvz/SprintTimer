#include "qt_storage_impl/QtTaskStorageWriter.h"


QtTaskStorageWriter::QtTaskStorageWriter(DBService& dbService)
    : dbService{dbService}
{
    addQueryId = dbService.prepare(
        "INSERT INTO todo_item (name, estimated_pomodoros, spent_pomodoros, "
        "completed, priority, last_modified, uuid) "
        "VALUES (:name, :estimated_pomodoros, :spent_pomodoros, :completed, "
        ":priority, :last_modified, :uuid); ");
    insertTagQueryId = dbService.prepare(
        "insert into task_tag_view(tagname, uuid) values(:tag, :uuid);");
    removeQueryId = dbService.prepare("");
    editQueryId = dbService.prepare("");
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
        addQueryId, ":name", QString::fromStdString(task.name()));
    dbService.bindValue(
        addQueryId, ":estimated_pomodoros", task.estimatedPomodoros());
    dbService.bindValue(addQueryId, ":spent_pomodoros", task.spentPomodoros());
    dbService.bindValue(addQueryId, ":completed", task.isCompleted());
    dbService.bindValue(addQueryId, ":priority", 10000);
    dbService.bindValue(
        addQueryId, ":last_modified", QDateTime::currentDateTime());
    dbService.bindValue(addQueryId, ":uuid", uuid);
    dbService.executePrepared(addQueryId);
    for (const auto& tag : task.tags()) {
        dbService.bindValue(
            insertTagQueryId, ":tag", QString::fromStdString(tag));
        dbService.bindValue(insertTagQueryId, ":uuid", uuid);
        dbService.executePrepared(insertTagQueryId);
    }
}

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
