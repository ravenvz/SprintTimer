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
