#ifndef TIMEINTERVAL_H
#define TIMEINTERVAL_H

#include "DateTime.h"
#include <chrono>


/* Represent finite interval in time with start and finish points. */
struct TimeSpan {

    using SystemClock = std::chrono::system_clock::time_point;

    DateTime startTime;
    DateTime finishTime;

    enum class DayPart { Midnight, Night, Morning, Noon, Afternoon, Evening };

    /* Construct from chrono time_point. */
    TimeSpan(SystemClock start, SystemClock finish);

    /* Construct from DateTime start and finish points. */
    TimeSpan(const DateTime& start, const DateTime& finish);

    /* Construct from std::time_t start and finish points. */
    TimeSpan(std::time_t start,
                 std::time_t finish,
                 int offsetFromUtcInSeconds = 0);

    // TODO remove when got rid of Qt containers
    TimeSpan();

    /* Return interval size in days as unsigned integer.
     *
     * It doesn't matter if start point is further in time compared to
     * finish point. */
    unsigned sizeInDays() const;

    /* Return enum representing part of the day.
     *
     * Day has 6 4-hour parts:
     *      Midnight  22:00 - 2:00
     *      Nigth      2:00 - 6:00
     *      Morning    6:00 - 10:00
     *      Noon      10:00 - 14:00
     *      Afternoon 14:00 - 18:00
     *      Evening   18:00 - 22:00
     */
    DayPart getDayPart() const;

    /* Return name of the day part given as unsigned integer as string. */
    static std::string dayPartName(unsigned dayPart);

    /* Return name of the given DayPart as string. */
    static std::string dayPartName(DayPart dayPart);

    /* Return hours corresponding to DayPart as string. */
    static std::string dayPartHours(unsigned dayPart);

    /* Return hours corresponding to DayPart as string. */
    static std::string dayPartHours(DayPart dayPart);

    /* Return time representation of interval as "HH:mm - HH:mm". */
    std::string toTimeString() const;
};

/* Return absolute number of days between startTime of this TimeSpan and
 * startTime of other TimeSpan. */
unsigned startDateAbsDiff(const TimeSpan& one, const TimeSpan& other);


#endif /* end of include guard: TIMEINTERVAL_H */
