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

    TimeInterval(SystemClock start, SystemClock finish)
        : startTime{DateTime{std::move(start)}}
        , finishTime{DateTime{std::move(finish)}}
    {
    }

    TimeInterval(const DateTime& start, const DateTime& finish)
        : startTime{start}
        , finishTime{finish}
    {
    }

    TimeInterval(std::time_t start,
                 std::time_t finish,
                 int offsetFromUtcInSeconds = 0)
        : startTime{DateTime::fromTime_t(start, offsetFromUtcInSeconds)}
        , finishTime{DateTime::fromTime_t(finish, offsetFromUtcInSeconds)}
    {
    }

    // TODO remove when got rid of Qt containers
    TimeInterval()
        : startTime{DateTime::currentDateTime()}
        , finishTime{DateTime::currentDateTime()}
    {
    }

    DateTime startTime;
    DateTime finishTime;

    enum class DayPart { MIDNIGHT, NIGHT, MORNING, NOON, AFTERNOON, EVENING };

    DayPart getDayPart() const
    {
        auto hour = startTime.hour();

        if (22 < hour || hour <= 2) {
            return DayPart::MIDNIGHT;
        }
        else if (2 < hour && hour <= 6) {
            return DayPart::NIGHT;
        }
        else if (6 < hour && hour <= 10) {
            return DayPart::MORNING;
        }
        else if (10 < hour && hour <= 14) {
            return DayPart::NOON;
        }
        else if (14 < hour && hour <= 18) {
            return DayPart::AFTERNOON;
        }
        else {
            return DayPart::EVENING;
        }
    }

    static QString dayPartName(int dayPart)
    {
        return dayPartName(static_cast<DayPart>(dayPart));
    }

    static QString dayPartName(DayPart dayPart)
    {
        switch (dayPart) {
        case DayPart::MIDNIGHT:
            return "Midnight";
        case DayPart::NIGHT:
            return "Night";
        case DayPart::MORNING:
            return "Morning";
        case DayPart::NOON:
            return "Noon";
        case DayPart::AFTERNOON:
            return "Afternoon";
        case DayPart::EVENING:
            return "Evening";
        }
        return "Invalid";
    }

    static QString dayPartHours(int dayPart)
    {
        return dayPartHours(static_cast<DayPart>(dayPart));
    }

    static QString dayPartHours(DayPart dayPart)
    {
        switch (dayPart) {
        case DayPart::MIDNIGHT:
            return "22:00 - 2:00";
        case DayPart::NIGHT:
            return "2:00 - 6:00";
        case DayPart::MORNING:
            return "6:00 - 10:00";
        case DayPart::NOON:
            return "10:00 - 14:00";
        case DayPart::AFTERNOON:
            return "14:00 - 18:00";
        case DayPart::EVENING:
            return "18:00 - 22:00";
        }
        return "Invalid";
    }

    QString toTimeString() const
    {
        std::string res{startTime.toTimeString()};
        res += " - ";
        res += finishTime.toTimeString();

        return QString::fromStdString(res);
    }
};

#endif /* end of include guard: TIMEINTERVAL_H */
