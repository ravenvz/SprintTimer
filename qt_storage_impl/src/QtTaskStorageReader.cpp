#include "qt_storage_impl/QtTaskStorageReader.h"


QtTaskStorageReader::QtTaskStorageReader(DBService& dbService)
    : dbService{dbService}
{
    mFinishedQueryId = dbService.prepare(
        "SELECT "
        "id, "
        "name, "
        "estimated_pomodoros, "
        "spent_pomodoros, "
        "priority, "
        "completed ,"
        "tags, "
        "last_modified, "
        "uuid "
        "FROM task_view "
        "WHERE completed = 1 AND last_modified >= (:start_date) "
        "AND last_modified <= (:end_date) "
        "ORDER BY last_modified;");
    connect(&dbService,
            &DBService::results,
            this,
            &QtTaskStorageReader::onResultsReceived);
}

void QtTaskStorageReader::requestTasks(const TimeSpan& timeSpan,
                                       Handler handler)
{
}

void QtTaskStorageReader::requestUnfinishedTasks(Handler handler)
{
    // TODO this can potentially end badly if we have multiple clients
    this->handler = handler;
    mUnfinishedQueryId = dbService.executeQuery(
        "SELECT "
        "id, "
        "name, "
        "estimated_pomodoros, "
        "spent_pomodoros, "
        "priority, "
        "completed, "
        "tags, "
        "last_modified, "
        "uuid "
        "FROM task_view "
        "WHERE completed = 0 OR last_modified > datetime('now', '-1 day') "
        "ORDER BY priority;");
}

void QtTaskStorageReader::requestFinishedTasks(const TimeSpan& timeSpan,
                                               Handler handler)
{
    // TODO this can potentially end badly if we have multiple clients
    this->handler = handler;

    DateTime start = timeSpan.startTime;
    DateTime finish = timeSpan.finishTime;

    dbService.bind(mFinishedQueryId,
                   ":start_date",
                   QVariant(QString::fromStdString(start.yyyymmddString())));
    dbService.bind(mFinishedQueryId,
                   ":end_date",
                   QVariant(QString::fromStdString(finish.yyyymmddString())));
    dbService.executePrepared(mFinishedQueryId);
}

void QtTaskStorageReader::onResultsReceived(
    long long queryId, const std::vector<QSqlRecord>& records)
{
    if (queryId != mUnfinishedQueryId && queryId != mFinishedQueryId) {
        return;
    }
    Items tasks;
    std::transform(
        records.cbegin(),
        records.cend(),
        std::back_inserter(tasks),
        [&](const auto& elem) { return this->taskFromQSqlRecord(elem); });
    handler(tasks);
}

TodoItem QtTaskStorageReader::taskFromQSqlRecord(const QSqlRecord& record)
{
    std::string name{columnData(record, Column::Name).toString().toStdString()};
    std::string uuid{columnData(record, Column::Uuid).toString().toStdString()};
    int estimatedPomodoros{
        columnData(record, Column::EstimatedPomodoros).toInt()};
    int spentPomodoros{columnData(record, Column::SpentPomodoros).toInt()};
    QStringList qTags{columnData(record, Column::Tags)
                          .toString()
                          .split(",", QString::SkipEmptyParts)};
    std::list<std::string> tags;
    std::transform(qTags.cbegin(),
                   qTags.cend(),
                   std::back_inserter(tags),
                   [](const auto& tag) { return tag.toStdString(); });
    bool finished{columnData(record, Column::Completed).toBool()};
    QDateTime qLastModified{
        columnData(record, Column::LastModified).toDateTime()};
    int offsetFromUtcInSeconds{qLastModified.offsetFromUtc()};
    DateTime lastModified = DateTime::fromTime_t(qLastModified.toTime_t(),
                                                 offsetFromUtcInSeconds);
    return TodoItem{name,
                    estimatedPomodoros,
                    spentPomodoros,
                    uuid,
                    tags,
                    finished,
                    lastModified};
}

QVariant QtTaskStorageReader::columnData(const QSqlRecord& record,
                                         Column column)
{
    return record.value(static_cast<int>(column));
}
