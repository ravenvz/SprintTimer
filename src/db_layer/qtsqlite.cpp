#include "qtsqlite.h"


QtSqlitePomodoroYearRangeReader::QtSqlitePomodoroYearRangeReader(
    DBService& dbService, Handler handler)
    : dbService{dbService}
    , handler{handler}
{
    connect(&dbService,
            &DBService::results,
            this,
            &QtSqlitePomodoroYearRangeReader::onResultsReceived);
}

void QtSqlitePomodoroYearRangeReader::requestYearRange()
{
    dbService.executeQuery("select distinct strftime('%Y', start_time) "
                           "from pomodoro order by start_time;");
}

void QtSqlitePomodoroYearRangeReader::onResultsReceived(
    const std::vector<QSqlRecord>& records)
{
    std::vector<std::string> range;
    for (const auto& record : records) {
        range.push_back(record.value(0).toString().toStdString());
    }
    handler(range);
}


QtSqlitePomodoroStorageReader::QtSqlitePomodoroStorageReader(
    DBService& dbService)
    : dbService{dbService}
{
    dbService.prepare(queryId,
                      "select * from pomodoro_view "
                      "where date(start_time) >= (:startTime) "
                      "and date(start_time) <= (:finishTime)");
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

    dbService.bind(queryId,
                   ":startTime",
                   QVariant(QString::fromStdString(start.yyyymmddString())));
    dbService.bind(queryId,
                   ":finishTime",
                   QVariant(QString::fromStdString(finish.yyyymmddString())));

    // QString query{
    //     QString("select * from pomodoro_view "
    //             "where date(start_time) >= '%1' and date(start_time) <=
    //             '%2'")
    //         .arg(QString::fromStdString(start.yyyymmddString()))
    //         .arg(QString::fromStdString(finish.yyyymmddString()))};
    dbService.executePrepared(queryId);
}

void QtSqlitePomodoroStorageReader::onResultsReceived(
    const std::vector<QSqlRecord>& records)
{
    std::vector<Pomodoro> pomodoros;
    std::transform(
        records.cbegin(),
        records.cend(),
        std::back_inserter(pomodoros),
        [&](const auto& elem) { return pomodoroFromQSqlRecord(elem); });
    handler(pomodoros);
}

Pomodoro
QtSqlitePomodoroStorageReader::pomodoroFromQSqlRecord(const QSqlRecord& record)
{
    QString name{columnData(record, Columns::Name).toString()};
    QDateTime start = columnData(record, Columns::StartTime).toDateTime();
    QDateTime finish = columnData(record, Columns::FinishTime).toDateTime();
    int offsetFromUtcInSeconds{start.offsetFromUtc()};
    TimeSpan timeSpan{
        start.toTime_t(), finish.toTime_t(), offsetFromUtcInSeconds};
    QStringList qTags{columnData(record, Columns::Tags).toString().split(",")};
    std::list<std::string> tags;
    std::transform(qTags.cbegin(),
                   qTags.cend(),
                   std::back_inserter(tags),
                   [](const auto& tag) { return tag.toStdString(); });
    return Pomodoro{name.toStdString(), timeSpan, tags};
}

QVariant QtSqlitePomodoroStorageReader::columnData(const QSqlRecord& record,
                                                   Columns column)
{
    return record.value(static_cast<int>(column));
}
