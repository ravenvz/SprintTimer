#include "qt_storage_impl/QtSqlitePomodoroYearRangeReader.h"


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
    dbService.executeQuery(mQueryId,
                           "select distinct strftime('%Y', start_time) "
                           "from pomodoro order by start_time;");
}

void QtSqlitePomodoroYearRangeReader::onResultsReceived(
    const QString& queryId, const std::vector<QSqlRecord>& records)
{
    if (mQueryId != queryId) {
        return;
    }
    std::vector<std::string> range;
    for (const auto& record : records) {
        range.push_back(record.value(0).toString().toStdString());
    }
    handler(range);
}
