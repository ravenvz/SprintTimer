#ifndef DATETIME_H_RTJVB37W
#define DATETIME_H_RTJVB37W


// TODO fix weird import
// #include "../../lib/date/date.h"
#include "date.h"
#include <sstream>


/* Convert std::tm to std::chrono::timepoint. */
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


/* Provides date and time functions.
 *
 * It is a wrapper around Howard Hinnant's date library
 * https://github.com/HowardHinnant */
class DateTime {

public:
    DateTime(std::chrono::system_clock::time_point timepoint);

    static DateTime fromYMD(int year, int month, int day);

    static DateTime fromTime_t(std::time_t timeT, int offsetFromUtcInSeconds);

    static DateTime currentDateTime();

    /* Not thread-safe */
    static DateTime currentDateTimeLocal();

    /* Return std::time_t representation. */
    std::time_t toTime_t() const;

    /* Return DateTime object that is (positive or negative) integer number of
     * days apart from current. */
    DateTime addDays(int days) const;

    /* Return DateTime object that is (positive or negative) integer number of
     * months apart from current. */
    DateTime addMonths(int months) const;

    /* Return distance in days to other DateTime object.
     *
     * If other DateTime object is behind in time, result will be negative.
     *
     * Number of days is a number of times midnight is encountered between
     * two DateTime objects. So distance between 11.04.2016 23:59 and
     * 12.04.2016 00:01 is equal to one day. */
    int daysTo(const DateTime& other) const;

    /* Return chrono time_point. */
    std::chrono::system_clock::time_point chronoTimepoint() const;

    /* Return year as integer. */
    int year() const;

    /* Return month as unsigned integer in [1, 12]. */
    unsigned month() const;

    /* Return day as unsigned integer in [1, 31]. */
    unsigned day() const;

    /* Return hours since midnight in 24-h format. */
    long hour() const;

    /* Return minutes since the start of the hour. */
    long minute() const;

    /* Return seconds since the start of the minute. */
    long second() const;

    /* Return day of week in [0, 6].
     *
     * 0 corresponds to Sunday. */
    unsigned dayOfWeek() const;

    /* Return string reprentation of time in HH:mm format. */
    std::string toTimeString() const;

    std::string yyyymmddString() const;

    std::string ddmmyyyyString() const;

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
