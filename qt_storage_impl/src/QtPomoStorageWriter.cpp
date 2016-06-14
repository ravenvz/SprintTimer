#include "qt_storage_impl/QtPomoStorageWriter.h"
#include "qt_common/DateTimeConverter.h"


QtPomoStorageWriter::QtPomoStorageWriter(DBService& dbService)
    : dbService{dbService}
{
    addQueryId = dbService.prepare(
        "insert into pomodoro_view(todo_uuid, start_time, finish_time, uuid) "
        "values(:todo_uuid, :startTime, :finishTime, :uuid);");
    removeQueryId
        = dbService.prepare("delete from pomodoro_view where uuid = (:uuid);");
}

void QtPomoStorageWriter::save(const Pomodoro& pomodoro)
{
    QDateTime startTime
        = DateTimeConverter::qDateTime(pomodoro.timeSpan().startTime);
    QDateTime finishTime
        = DateTimeConverter::qDateTime(pomodoro.timeSpan().finishTime);
    dbService.bind(addQueryId,
                   ":todo_uuid",
                   QVariant(QString::fromStdString(pomodoro.taskUuid())));
    dbService.bind(addQueryId, ":startTime", QVariant(startTime));
    dbService.bind(addQueryId, ":finishTime", QVariant(finishTime));
    dbService.bind(
        addQueryId, ":uuid", QVariant(QString::fromStdString(pomodoro.uuid())));
    dbService.executePrepared(addQueryId);
}

void QtPomoStorageWriter::remove(const Pomodoro& pomodoro)
{
    dbService.bind(removeQueryId,
                   ":uuid",
                   QVariant(QString::fromStdString(pomodoro.uuid())));
    dbService.executePrepared(removeQueryId);
}
