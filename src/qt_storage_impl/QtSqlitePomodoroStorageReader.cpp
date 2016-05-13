#include "qt_storage_impl/QtSqlitePomodoroStorageReader.h"


QtSqlitePomodoroStorageReader::QtSqlitePomodoroStorageReader(
    DBService& dbService)
    : dbService{dbService}
{
    dbService.prepare(
        queryId,
        "select id, todo_id, name, tags, start_time, finish_time, uuid "
        "from pomodoro_view "
        "where date(start_time) >= (:startTime) "
        "and date(start_time) <= (:finishTime)");
    connect(&dbService,
            &DBService::results,
            this,
            &QtSqlitePomodoroStorageReader::onResultsReceived);
    connect(&dbService,
            &DBService::error,
            this,
            &QtSqlitePomodoroStorageReader::onError);
}

void QtSqlitePomodoroStorageReader::requestItems(const TimeSpan& timeSpan,
                                                 Handler handler)
{
    this->handler = handler;
    DateTime start = timeSpan.startTime;
    DateTime finish = timeSpan.finishTime;

    dbService.bind(queryId,
                   ":startTime",
                   QVariant(QString::fromStdString(start.yyyymmddString())));
    dbService.bind(queryId,
                   ":finishTime",
                   QVariant(QString::fromStdString(finish.yyyymmddString())));

    dbService.executePrepared(queryId);
}

void QtSqlitePomodoroStorageReader::onResultsReceived(
    const std::vector<QSqlRecord>& records)
{
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
    std::list<std::string> tags;
    std::transform(qTags.cbegin(),
                   qTags.cend(),
                   std::back_inserter(tags),
                   [](const auto& tag) { return tag.toStdString(); });
    std::cout << uuid << std::endl;
    return Pomodoro{name.toStdString(), timeSpan, tags, uuid};
}

QVariant QtSqlitePomodoroStorageReader::columnData(const QSqlRecord& record,
                                                   Columns column)
{
    return record.value(static_cast<int>(column));
}

void QtSqlitePomodoroStorageReader::onError(const QString& error)
{
    qDebug() << error;
}

