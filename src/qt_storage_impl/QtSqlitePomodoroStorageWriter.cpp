#include "qt_storage_impl/QtSqlitePomodoroStorageWriter.h"


QtSqlitePomodoroStorageWriter::QtSqlitePomodoroStorageWriter(
    DBService& dbService)
    : dbService{dbService}
{
    connect(&dbService,
            &DBService::error,
            this,
            &QtSqlitePomodoroStorageWriter::onError);
    dbService.prepare(
        addQueryId,
        "insert into pomodoro_view (todo_id, start_time, finish_time, uuid) "
        "values (:todo_id, :start_time, :finishTime, :uuid);");
    dbService.prepare(removeQueryId,
                      "delete from pomodoro_view where uuid = (:uuid);");
}

void QtSqlitePomodoroStorageWriter::save(const Pomodoro& pomodoro,
                                         long long taskId)
{
    QDateTime startTime = QDateTime::fromTime_t(
        static_cast<unsigned>(pomodoro.timeSpan().startTime.toTime_t()));
    QDateTime finishTime = QDateTime::fromTime_t(
        static_cast<unsigned>(pomodoro.timeSpan().finishTime.toTime_t()));
    dbService.bind(addQueryId, ":todo_id", QVariant(taskId));
    dbService.bind(addQueryId, ":start_time", QVariant(startTime));
    dbService.bind(addQueryId, ":finishTime", QVariant(finishTime));
    dbService.bind(
        addQueryId, ":uuid", QVariant(QString::fromStdString(pomodoro.uuid())));
    dbService.executePrepared(addQueryId);
}

void QtSqlitePomodoroStorageWriter::remove(const Pomodoro& pomodoro)
{
    dbService.bind(removeQueryId,
                   ":uuid",
                   QVariant(QString::fromStdString(pomodoro.uuid())));
    dbService.executePrepared(removeQueryId);
}

void QtSqlitePomodoroStorageWriter::onError(const QString& errorMessage)
{
    throw std::runtime_error("All fucked up");
}
