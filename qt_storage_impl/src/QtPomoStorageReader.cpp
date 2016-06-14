#include "qt_storage_impl/QtPomoStorageReader.h"
#include "qt_common/DateTimeConverter.h"


QtPomoStorageReader::QtPomoStorageReader(DBService& dbService)
    : dbService{dbService}
{
    mQueryId = dbService.prepare(
        "select id, todo_uuid, name, tags, start_time, finish_time, uuid "
        "from pomodoro_view "
        "where date(start_time) >= (:startTime) "
        "and date(start_time) <= (:finishTime) "
        "order by start_time");
    connect(&dbService,
            &DBService::results,
            this,
            &QtPomoStorageReader::onResultsReceived);
}

void QtPomoStorageReader::requestItems(const TimeSpan& timeSpan,
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

void QtPomoStorageReader::onResultsReceived(
    long long queryId, const std::vector<QSqlRecord>& records)
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

Pomodoro QtPomoStorageReader::pomodoroFromQSqlRecord(const QSqlRecord& record)
{
    QString name{columnData(record, Columns::Name).toString()};
    QDateTime start = columnData(record, Columns::StartTime).toDateTime();
    QDateTime finish = columnData(record, Columns::FinishTime).toDateTime();
    TimeSpan timeSpan{DateTimeConverter::dateTime(start),
                      DateTimeConverter::dateTime(finish)};
    std::string uuid
        = columnData(record, Columns::Uuid).toString().toStdString();
    QStringList tagNames{columnData(record, Columns::Tags)
                             .toString()
                             .split(",", QString::SkipEmptyParts)};
    std::string taskUuid
        = columnData(record, Columns::TodoUuid).toString().toStdString();
    std::list<Tag> tags;
    std::transform(tagNames.cbegin(),
                   tagNames.cend(),
                   std::back_inserter(tags),
                   [](const auto& name) { return Tag{name.toStdString()}; });
    return Pomodoro{name.toStdString(), timeSpan, tags, uuid, taskUuid};
}

QVariant QtPomoStorageReader::columnData(const QSqlRecord& record,
                                         Columns column)
{
    return record.value(static_cast<int>(column));
}
