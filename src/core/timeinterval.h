#ifndef TIMEINTERVAL_H
#define TIMEINTERVAL_H

#include <QDateTime>
#include <sstream>
#include <cstring>
#include <memory>
#include <ctime>
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


// template <class Clock, class Duration = typename Clock::duration>
class DateTime {

public:
    DateTime(std::chrono::system_clock::time_point timepoint)
        : time{timepoint}
        , daypoint{date::floor<date::days>(time)}
        , ymd{date::year_month_day(daypoint)}
        , tod{date::make_time(time - daypoint)}
    {
    }

    static DateTime fromYMD(int year, int month, int day)
    {
        date::year_month_day dateYMD{date::year(year) / month / day};
        if (!dateYMD.ok())
            throw std::runtime_error("Invalid date");
        std::chrono::system_clock::time_point tp = date::day_point(dateYMD);
        return DateTime{tp};
    }

    static DateTime fromTime_t(std::time_t timeT,
                               int offsetFromUtcInSeconds = 0)
    {
        return DateTime{std::chrono::system_clock::from_time_t(timeT)
                        + std::chrono::seconds{offsetFromUtcInSeconds}};
    }

    static DateTime currentDateTime()
    {
        return DateTime{std::chrono::system_clock::now()};
    }

    static DateTime currentDateTimeLocal()
    {
        auto timepoint = std::chrono::system_clock::now();
        std::time_t t = std::chrono::system_clock::to_time_t(timepoint);
        std::tm localTime = *std::localtime(&t);
        to_time_point(localTime, timepoint);
        return DateTime{timepoint};
    }


    std::time_t toTime_t() const
    {
        return std::chrono::system_clock::to_time_t(time);
    }

    DateTime addDays(int days) const
    {
        return DateTime{time + date::days{days}};
    }

    int daysTo(const DateTime& other)
    {
        return (other.ymd.day() - this->ymd.day()).count();
    }

    std::chrono::system_clock::time_point chronoTimepoint() const
    {
        return time;
    }

    int year() const { return int(ymd.year()); }

    unsigned month() const { return unsigned(ymd.month()); }

    unsigned day() const { return unsigned(ymd.day()); }

    long hour() const { return tod.hours().count(); }

    long minute() const { return tod.minutes().count(); }

    long second() const { return tod.seconds().count(); }

    date::time_of_day<std::chrono::system_clock::duration> time_of_day() const
    {
        return tod;
    }

    std::string toTimeString() const
    {
        using namespace date;
        std::stringstream ss;
        auto tp = floor<std::chrono::minutes>(time);
        auto dp = floor<date::days>(time);
        ss << make_time(tp - dp);
        return ss.str();
    }

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

    // QString toString() const
    // {
    //     using namespace date;
    //     std::stringstream ss;
    //     ss << startTime;
    //     ss << " - ";
    //     ss << finishTime;
    //     return QString::fromStdString(ss.str());
    //     // return QString("%1 - %2")
    //     //     .arg(startTime.toString())
    //     //     .arg(finishTime.toString());
    // }
    //
    QString toTimeString() const
    {
        // using namespace date;
        // auto daypoint_start = date::floor<date::days>(startTime);
        // auto timeofday_start = date::make_time(startTime - daypoint_start);
        // auto daypoint_finish = date::floor<date::days>(finishTime);
        // auto timeofday_finish = date::make_time(finishTime -
        // daypoint_finish);
        std::string res{startTime.toTimeString()};
        res += " - ";
        res += finishTime.toTimeString();

        return QString::fromStdString(res);

        // return QString("%1 - %2")
        //     .arg(startTime.toString("HH:mm"))
        //     .arg(finishTime.toString("HH:mm"));
    }
};

#endif /* end of include guard: TIMEINTERVAL_H */
