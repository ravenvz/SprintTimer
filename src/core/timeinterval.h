#ifndef TIMEINTERVAL_H
#define TIMEINTERVAL_H

#include <chrono>
#include "DateTime.h"


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

    unsigned sizeInDays() const;

    DayPart getDayPart() const;

    static std::string dayPartName(unsigned dayPart);

    static std::string dayPartName(DayPart dayPart);

    static std::string dayPartHours(unsigned dayPart);

    static std::string dayPartHours(DayPart dayPart);

    std::string toTimeString() const;
};

/* Return absolute number of days between startTime of this TimeInterval and
 * startTime of other TimeInterval
 */
unsigned startDateAbsDiff(const TimeInterval& one, const TimeInterval& other);


#endif /* end of include guard: TIMEINTERVAL_H */
