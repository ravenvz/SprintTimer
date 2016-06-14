#include "qt_storage_impl/QtPomoDistributionReader.h"
#include "qt_common/DateTimeConverter.h"


DistributionReaderBase::DistributionReaderBase(DBService& dbService)
    : dbService{dbService}
{
    connect(&dbService,
            &DBService::results,
            this,
            &DistributionReaderBase::onResultsReceived);
}

void DistributionReaderBase::requestDailyDistribution(const TimeSpan& timeSpan,
                                                      Handler handler)
{
    this->handler = handler;
    QDate from = DateTimeConverter::qDate(timeSpan.startTime);
    QDate now = DateTimeConverter::qDate(timeSpan.finishTime);

    dbService.bind(mQueryId, ":start_date", from);
    dbService.bind(mQueryId, ":end_date", now);
    dbService.executePrepared(mQueryId);
}

void DistributionReaderBase::onResultsReceived(
    long long queryId, const std::vector<QSqlRecord>& records)
{
    if (mQueryId != queryId) {
        return;
    }
    std::vector<int> pomodoroCount;
    std::transform(records.cbegin(),
                   records.cend(),
                   std::back_inserter(pomodoroCount),
                   [](const auto& elem) { return elem.value(0).toInt(); });
    // Distribution<int> dailyDistribution{std::move(pomodoroCount)};
    handler(Distribution<int>{std::move(pomodoroCount)});
}

QtPomoDailyDistributionReader::QtPomoDailyDistributionReader(
    DBService& dbService)
    : DistributionReaderBase{dbService}
{
    mQueryId = dbService.prepare(
        "select count(pomodoro.id) "
        "from calendar left join pomodoro "
        "on date(start_time) = dt "
        "where dt > date(:start_date) and dt <= date(:end_date)"
        "group by date(dt) "
        "order by dt");
}

QtPomoWeeklyDistributionReader::QtPomoWeeklyDistributionReader(
    DBService& dbService)
    : DistributionReaderBase{dbService}
{
    mQueryId = dbService.prepare(
        "select count(start_time) "
        "from calendar left join pomodoro "
        "on date(start_time) = dt "
        "where dt > (:start_date) and dt <= (:end_date) "
        "group by (strftime('%j', date(dt, '-3 days', 'weekday "
        "4')) - 1) / 7 + 1 "
        "order by dt");
}

QtPomoMonthlyDistributionReader::QtPomoMonthlyDistributionReader(
    DBService& dbService)
    : DistributionReaderBase{dbService}
{
    mQueryId
        = dbService.prepare("select count(start_time) "
                            "from calendar left join pomodoro "
                            "on date(start_time) = dt "
                            "where dt > (:start_date) and dt <= (:end_date) "
                            "group by strftime('%m', dt) "
                            "order by dt");
}
