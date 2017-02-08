/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of SprintTimer.
**
** SprintTimer is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** SprintTimer is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with SprintTimer.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef DATETIME_H_RTJVB37W
#define DATETIME_H_RTJVB37W


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
    tp = sys_days(ymd) + hours(t.tm_hour) + minutes(t.tm_min)
        + seconds(t.tm_sec);
}


/* Provides date and time functions.
 *
 * It is a wrapper around Howard Hinnant's date library
 * https://github.com/HowardHinnant */
class DateTime {

public:
    enum class Weekday {
        Monday,
        Tuesday,
        Wednesday,
        Thursday,
        Friday,
        Saturday,
        Sunday
    };

    DateTime(std::chrono::system_clock::time_point timepoint);

    static DateTime fromYMD(int year, int month, int day);

    static DateTime fromTime_t(std::time_t timeT, int offsetFromUtcInSeconds);

    static DateTime currentDateTime();

    /* Not thread-safe */
    static DateTime currentDateTimeLocal();

    /* Return std::time_t representation. */
    std::time_t toTime_t() const;

    /* Return DateTime object that is seconds apart from current.
     * Seconds can be negative. */
    DateTime addSeconds(long seconds) const;

    /* Return DateTime object that is minutes apart from current.
     * Minutes can be negative. */
    DateTime addMinutes(long minutes) const;

    /* Return DateTime object that is hours apart from current.
     * Hours can be negative. */
    DateTime addHours(long hours) const;

    /* Return DateTime object that is (positive or negative) integer number of
     * days apart from current. */
    DateTime addDays(long days) const;

    /* Return DateTime object that is (positive or negative) integer number of
     * months apart from current. */
    DateTime addMonths(long months) const;

    /* Return DateTime object that is (positive or negative) integer number of
     * years apart from current. */
    DateTime addYears(long years) const;

    /* Return distance in seconds to other DateTime object.
     *
     * If other DateTime object is behind in time, result will be negative. */
    long long secondsTo(const DateTime& other) const;

    /* Return distance in minutes to other DateTime object.
     *
     * If other DateTime object is behind in time, result will be negative. */
    long minutesTo(const DateTime& other) const;

    /* Return distance in hours to other DateTime object.
     *
     * If other DateTime object is behind in time, result will be negative. */
    long hoursTo(const DateTime& other) const;

    /* Return distance in days to other DateTime object.
     *
     * If other DateTime object is behind in time, result will be negative.
     *
     * Number of days is a number of times midnight is encountered between
     * two DateTime objects. So distance between 11.04.2016 23:59 and
     * 12.04.2016 00:01 is equal to one day. */
    long daysTo(const DateTime& other) const;

    /* Return distance in months to other DateTime object.
     *
     * If other DateTime object is behind in time, result will be negative. */
    long monthsTo(const DateTime& other) const;

    /* Return distance in months to other DateTime object.
     *
     * If other DateTime object is behind in time, result will be negative. */
    long yearsTo(const DateTime& other) const;

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
    long long second() const;

    /* Return day of week. */
    Weekday dayOfWeek() const;

    /* Return string representation of DateTime.
     * The format parameter determines the format of the result string.
     *
     * These expressions may be used for the date:
     *
     * d	the day as number without a leading zero (1 to 31)
     * dd	the day as number with a leading zero (01 to 31)
     * M	the month as number without a leading zero (1-12)
     * MM	the month as number with a leading zero (01-12)
     * yy	the year as two digit number (00-99)
     * yyyy	the year as four digit number
     *
     * These expressions may be used for the time:
     *
     * h	the hour without a leading zero (0 to 23 or 1 to 12 if AM/PM
     * display)
     * hh	the hour with a leading zero (00 to 23 or 01 to 12 if AM/PM
     * display)
     * m	the minute without a leading zero (0 to 59)
     * mm	the minute with a leading zero (00 to 59)
     * s	the second without a leading zero (0 to 59)
     * ss	the second with a leading zero (00 to 59)
     *
     * All other input characters will be ignored.
     * Any sequence of characters that are enclosed in single quotes will be
     * treated as text and not be used as an expression.
     */
    std::string toString(std::string format) const;

    friend inline bool operator==(const DateTime& dt1, const DateTime& dt2);

    friend inline bool operator!=(const DateTime& dt1, const DateTime& dt2);

    friend inline bool operator<(const DateTime& dt1, const DateTime& dt2);

    friend inline bool operator<=(const DateTime& dt1, const DateTime& dt2);

    friend inline bool operator>(const DateTime& dt1, const DateTime& dt2);

    friend inline bool operator>=(const DateTime& dt1, const DateTime& dt2);

private:
    std::chrono::system_clock::time_point time;
    date::sys_days daypoint;
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
