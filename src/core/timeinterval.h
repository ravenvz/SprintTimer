#ifndef TIMEINTERVAL_H
#define TIMEINTERVAL_H

#include <QDateTime>
#include <sstream>
// #include <cstring>
// #include <memory>
// #include <ctime>
#include "../../lib/date/date.h"


template <typename Clock, typename Duration>
void to_time_point(const std::tm& t,
                   std::chrono::time_point<Clock, Duration>& tp)
{
    using namespace std::chrono;
    using namespace date;
    int y = t.tm_year + 1900;
    auto ymd = year(y) / (t.tm_mon + 1) / t.tm_mday;
    if (!ymd.ok())
        throw std::runtime_error("Invalid date");
    tp = day_point(ymd) + hours(t.tm_hour) + minutes(t.tm_min)
        + seconds(t.tm_sec);
}


class DateTime {

public:
    DateTime(std::chrono::system_clock::time_point timepoint);

    static DateTime fromYMD(int year, int month, int day);

    static DateTime fromTime_t(std::time_t timeT, int offsetFromUtcInSeconds);

    static DateTime currentDateTime();

    /* Not thread-safe */
    static DateTime currentDateTimeLocal();

    std::time_t toTime_t() const;

    DateTime addDays(int days) const;

    int daysTo(const DateTime& other) const;

    std::chrono::system_clock::time_point chronoTimepoint() const;

    int year() const;

    unsigned month() const;

    unsigned day() const;

    long hour() const;

    long minute() const;

    long second() const;

    std::string toTimeString() const;

    friend inline bool operator==(const DateTime& dt1, const DateTime& dt2);

    friend inline bool operator!=(const DateTime& dt1, const DateTime& dt2);

private:
    std::chrono::system_clock::time_point time;
    date::day_point daypoint;
    date::year_month_day ymd;
    date::time_of_day<std::chrono::system_clock::duration> tod;
};

inline bool operator==(const DateTime& dt1, const DateTime& dt2)
{
    return dt1.time == dt1.time;
}

inline bool operator!=(const DateTime& dt1, const DateTime& dt2)
{
    return !(dt1 == dt2);
}


struct DateInterval {
    QDate startDate;
    QDate endDate;

    QString toString() const
    {
        return QString("%1 - %2")
            .arg(startDate.toString())
            .arg(endDate.toString());
    }

    int sizeInDays() const
    {
        return static_cast<int>(startDate.daysTo(endDate) + 1);
    }
};


struct TimeInterval {

    using SystemClock = std::chrono::system_clock::time_point;
    DateTime startTime;
    DateTime finishTime;
    enum class DayPart { MIDNIGHT, NIGHT, MORNING, NOON, AFTERNOON, EVENING };

    TimeInterval(SystemClock start, SystemClock finish);

    TimeInterval(const DateTime& start, const DateTime& finish);

    TimeInterval(std::time_t start,
                 std::time_t finish,
                 int offsetFromUtcInSeconds = 0);

    // TODO remove when got rid of Qt containers
    TimeInterval();

    DayPart getDayPart() const;

    static std::string dayPartName(int dayPart);

    static std::string dayPartName(DayPart dayPart);

    static std::string dayPartHours(int dayPart);

    static std::string dayPartHours(DayPart dayPart);

    std::string toTimeString() const;
};

#endif /* end of include guard: TIMEINTERVAL_H */
