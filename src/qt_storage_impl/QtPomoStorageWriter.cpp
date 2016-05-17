#include "qt_storage_impl/QtPomoStorageWriter.h"


QtPomoStorageWriter::QtPomoStorageWriter(
    DBService& dbService)
    : dbService{dbService}
{
    dbService.prepare(
        addQueryId,
        "insert into pomodoro_view(todo_uuid, start_time, finish_time, uuid) "
        "values(:todo_uuid, :startTime, :finishTime, :uuid);");
    dbService.prepare(removeQueryId,
                      "delete from pomodoro_view where uuid = (:uuid);");
}

void QtPomoStorageWriter::save(const Pomodoro& pomodoro)
{
    QDateTime startTime = QDateTime::fromTime_t(
        static_cast<unsigned>(pomodoro.timeSpan().startTime.toTime_t()));
    QDateTime finishTime = QDateTime::fromTime_t(
        static_cast<unsigned>(pomodoro.timeSpan().finishTime.toTime_t()));
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
