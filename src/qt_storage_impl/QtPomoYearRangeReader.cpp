#include "qt_storage_impl/QtPomoYearRangeReader.h"


QtPomoYearRangeReader::QtPomoYearRangeReader(DBService& dbService)
    : dbService{dbService}
{
    connect(&dbService,
            &DBService::results,
            this,
            &QtPomoYearRangeReader::onResultsReceived);
}

void QtPomoYearRangeReader::requestYearRange(Handler handler)
{
    this->handler = handler;
    mQueryId
        = dbService.executeQuery("select distinct strftime('%Y', start_time) "
                                 "from pomodoro order by start_time;");
}

void QtPomoYearRangeReader::onResultsReceived(
    long long queryId, const std::vector<QSqlRecord>& records)
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
