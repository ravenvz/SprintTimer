#ifndef DATEINTERVAL_H_DO5P2YKX
#define DATEINTERVAL_H_DO5P2YKX

#include <QDate>
#include "core/DateTime.h"
#include <iostream>

struct DateInterval {

    QDate startDate;
    QDate endDate;

    static DateInterval fromTimeInterval(const TimeInterval& interval)
    {
        return DateInterval{
            QDateTime::fromTime_t(
                static_cast<unsigned>(interval.startTime.toTime_t()))
                .date(),
            QDateTime::fromTime_t(
                static_cast<unsigned>(interval.finishTime.toTime_t()))
                .date()};
    }

    QString toString() const
    {
        return QString("%1 - %2")
            .arg(startDate.toString())
            .arg(endDate.toString());
    }

    unsigned sizeInDays() const
    {
        if (startDate <= endDate) {
            return static_cast<unsigned>(startDate.daysTo(endDate) + 1);
        }
        else {
            return static_cast<unsigned>(endDate.daysTo(startDate) + 1);
        }
    }

    TimeInterval toTimeInterval() const
    {
        /* When QDateTime constructs from QDate, it sets time to 00:00,
         * which is treated like a previous day by chrono (or date lib,
         * it's hard to say...). That is why another day is added.
         * It might be better to make DateInterval dependent on date lib
         * instead of QDate TODO */
        return TimeInterval{QDateTime{startDate}.addDays(1).toTime_t(),
                            QDateTime{endDate}.addDays(1).toTime_t()};
    }
};

// std::ostream& operator<<(std::ostream& os, const DateInterval& interval)
// {
//     TimeInterval t = interval.toTimeInterval();
//     os << "DateInterval: " << t.startTime.day() << "." << t.startTime.month()
//        << "." << t.startTime.year() << " - " << t.finishTime.day() << "."
//        << t.finishTime.month() << "." << t.finishTime.year();
//     return os;
// }

#endif /* end of include guard: DATEINTERVAL_H_DO5P2YKX */
