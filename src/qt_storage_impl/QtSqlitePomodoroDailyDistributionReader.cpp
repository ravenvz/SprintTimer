#include "QtSqlitePomodoroDailyDistributionReader.h"
#include "utils/MathUtils.h"


DistributionReaderBase::DistributionReaderBase(DBService& dbService,
                                               QString queryId)
    : dbService{dbService}
    , mQueryId{std::move(queryId)}
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

void DistributionReaderBase::onResultsReceived(
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
    // Distribution<int> dailyDistribution{std::move(pomodoroCount)};
    handler(Distribution<int>{std::move(pomodoroCount)});
}

QtSqlitePomodoroDailyDistributionReader::
    QtSqlitePomodoroDailyDistributionReader(DBService& dbService)
    : DistributionReaderBase{dbService, QString{"RequestDailyDistribution"}}
{
    dbService.prepare(mQueryId,
                      "select count(pomodoro.id) "
                      "from calendar left join pomodoro "
                      "on date(start_time) = dt "
                      "where dt > date(:start_date) and dt <= date(:end_date)"
                      "group by date(dt) "
                      "order by dt");
}

QtSqlitePomodoroWeeklyDistributionReader::
    QtSqlitePomodoroWeeklyDistributionReader(DBService& dbService)
    : DistributionReaderBase{dbService, QString{"RequestWeeklyDistribution"}}
{
    dbService.prepare(mQueryId,
                      "select count(start_time) "
                      "from calendar left join pomodoro "
                      "on date(start_time) = dt "
                      "where dt > (:start_date) and dt <= (:end_date) "
                      "group by (strftime('%j', date(dt, '-3 days', 'weekday "
                      "4')) - 1) / 7 + 1 "
                      "order by dt");
}

QtSqlitePomodoroMonthlyDistributionReader::
    QtSqlitePomodoroMonthlyDistributionReader(DBService& dbService)
    : DistributionReaderBase{dbService, QString{"RequestMonthlyDistribution"}}
{
    dbService.prepare(mQueryId,
                      "select count(start_time) "
                      "from calendar left join pomodoro "
                      "on date(start_time) = dt "
                      "where dt > (:start_date) and dt <= (:end_date) "
                      "group by strftime('%m', dt) "
                      "order by dt");
}
