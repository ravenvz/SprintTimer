#ifndef DATETIME_H_RTJVB37W
#define DATETIME_H_RTJVB37W


#include <sstream>
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

    unsigned dayOfWeek() const;

    std::string toTimeString() const;

    friend inline bool operator==(const DateTime& dt1, const DateTime& dt2);

    friend inline bool operator!=(const DateTime& dt1, const DateTime& dt2);

    friend inline bool operator<(const DateTime& dt1, const DateTime& dt2);

    friend inline bool operator<=(const DateTime& dt1, const DateTime& dt2);

    friend inline bool operator>(const DateTime& dt1, const DateTime& dt2);

    friend inline bool operator>=(const DateTime& dt1, const DateTime& dt2);

private:
    std::chrono::system_clock::time_point time;
    date::day_point daypoint;
    date::year_month_day ymd;
    date::time_of_day<std::chrono::system_clock::duration> tod;
};

std::ostream& operator<<(std::ostream& os, const DateTime& dt);

inline bool operator==(const DateTime& lhs, const DateTime& rhs)
{
    return lhs.time == rhs.time;
}

inline bool operator!=(const DateTime& lhs, const DateTime& rhs)
{
    return !(lhs == rhs);
}

inline bool operator<(const DateTime& lhs, const DateTime& rhs)
{
    return lhs.time < rhs.time;
}

inline bool operator>(const DateTime& lhs, const DateTime& rhs)
{
    return rhs < lhs;
}
inline bool operator<=(const DateTime& lhs, const DateTime& rhs)
{
    return !(lhs > rhs);
}
inline bool operator>=(const DateTime& lhs, const DateTime& rhs)
{
    return !(lhs < rhs);
}

#endif /* end of include guard: DATETIME_H_RTJVB37W */
