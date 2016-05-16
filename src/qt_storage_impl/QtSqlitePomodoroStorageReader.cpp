#include "qt_storage_impl/QtSqlitePomodoroStorageReader.h"


QtSqlitePomodoroStorageReader::QtSqlitePomodoroStorageReader(
    DBService& dbService)
    : dbService{dbService}
{
    dbService.prepare(
        mQueryId,
        "select id, todo_uuid, name, tags, start_time, finish_time, uuid "
        "from pomodoro_view "
        "where date(start_time) >= (:startTime) "
        "and date(start_time) <= (:finishTime) "
        "order by start_time");
    connect(&dbService,
            &DBService::results,
            this,
            &QtSqlitePomodoroStorageReader::onResultsReceived);
}

void QtSqlitePomodoroStorageReader::requestItems(const TimeSpan& timeSpan,
                                                 Handler handler)
{
    this->handler = handler;
    DateTime start = timeSpan.startTime;
    DateTime finish = timeSpan.finishTime;

    dbService.bind(mQueryId,
                   ":startTime",
                   QVariant(QString::fromStdString(start.yyyymmddString())));
    dbService.bind(mQueryId,
                   ":finishTime",
                   QVariant(QString::fromStdString(finish.yyyymmddString())));

    dbService.executePrepared(mQueryId);
}

void QtSqlitePomodoroStorageReader::onResultsReceived(
    const QString& queryId, const std::vector<QSqlRecord>& records)
{
    if (mQueryId != queryId) {
        return;
    }
    Items pomodoros;
    std::transform(
        records.cbegin(),
        records.cend(),
        std::back_inserter(pomodoros),
        [&](const auto& elem) { return this->pomodoroFromQSqlRecord(elem); });
    handler(pomodoros);
}

Pomodoro
QtSqlitePomodoroStorageReader::pomodoroFromQSqlRecord(const QSqlRecord& record)
{
    QString name{columnData(record, Columns::Name).toString()};
    QDateTime start = columnData(record, Columns::StartTime).toDateTime();
    QDateTime finish = columnData(record, Columns::FinishTime).toDateTime();
    std::string uuid
        = columnData(record, Columns::Uuid).toString().toStdString();
    int offsetFromUtcInSeconds{start.offsetFromUtc()};
    TimeSpan timeSpan{
        start.toTime_t(), finish.toTime_t(), offsetFromUtcInSeconds};
    QStringList qTags{columnData(record, Columns::Tags).toString().split(",")};
    std::string taskUuid
        = columnData(record, Columns::TodoUuid).toString().toStdString();
    std::list<std::string> tags;
    std::transform(qTags.cbegin(),
                   qTags.cend(),
                   std::back_inserter(tags),
                   [](const auto& tag) { return tag.toStdString(); });
    return Pomodoro{name.toStdString(), timeSpan, tags, uuid, taskUuid};
}

QVariant QtSqlitePomodoroStorageReader::columnData(const QSqlRecord& record,
                                                   Columns column)
{
    return record.value(static_cast<int>(column));
}
