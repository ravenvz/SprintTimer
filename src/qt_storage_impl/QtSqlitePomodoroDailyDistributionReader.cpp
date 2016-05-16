#include "QtSqlitePomodoroDailyDistributionReader.h"
#include "utils/MathUtils.h"


QtSqlitePomodoroDailyDistributionReader::
    QtSqlitePomodoroDailyDistributionReader(DBService& dbService)
    : dbService{dbService}
{
    dbService.prepare(mQueryId,
                      "select count(pomodoro.id) "
                      "from calendar left join pomodoro "
                      "on date(start_time) = dt "
                      "where dt > date(:start_date) and dt <= date(:end_date)"
                      "group by date(dt) "
                      "order by dt");
    connect(&dbService,
            &DBService::results,
            this,
            &QtSqlitePomodoroDailyDistributionReader::onResultsReceived);
}

void QtSqlitePomodoroDailyDistributionReader::requestDailyDistribution(
    const TimeSpan& timeSpan, Handler handler)
{
    this->handler = handler;
    // QDate now = QDate::currentDate();
    QDate from = QDateTime::fromTime_t(
                     static_cast<unsigned>(timeSpan.startTime.toTime_t()))
                     .date();
    QDate now = QDateTime::fromTime_t(
                    static_cast<unsigned>(timeSpan.finishTime.toTime_t()))
                    .date();

    dbService.bind(mQueryId, ":start_date", from);
    dbService.bind(mQueryId, ":end_date", now);
    dbService.executePrepared(mQueryId);
}

void QtSqlitePomodoroDailyDistributionReader::onResultsReceived(
    const QString& queryId, const std::vector<QSqlRecord>& records)
{
    if (mQueryId != queryId) {
        return;
    }
    std::vector<int> pomodoroCount;
    std::transform(records.cbegin(),
                   records.cend(),
                   std::back_inserter(pomodoroCount),
                   [](const auto& elem) { return elem.value(0).toInt(); });
    Distribution<int> dailyDistribution{std::move(pomodoroCount)};
    handler(dailyDistribution);
}
