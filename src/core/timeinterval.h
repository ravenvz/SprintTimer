#ifndef TIMEINTERVAL_H
#define TIMEINTERVAL_H

#include <QDateTime>
#include <chrono>
#include "DateTime.h"


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
