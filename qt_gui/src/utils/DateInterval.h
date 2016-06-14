#ifndef DATEINTERVAL_H_DO5P2YKX
#define DATEINTERVAL_H_DO5P2YKX

#include "core/TimeSpan.h"
#include "qt_common/DateTimeConverter.h"
#include <QDate>
#include <iostream>

struct DateInterval {

    QDate startDate;
    QDate endDate;

    static DateInterval fromTimeSpan(const TimeSpan& timeSpan)
    {
        return DateInterval{DateTimeConverter::qDate(timeSpan.startTime),
                            DateTimeConverter::qDate(timeSpan.finishTime)};
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

    TimeSpan toTimeSpan() const
    {
        /* When QDateTime constructs from QDate, it sets time to 00:00,
         * which is treated like a previous day by chrono (or date lib,
         * it's hard to say...). That is why another day is added.
         * It might be better to make DateInterval dependent on date lib
         * instead of QDate TODO */
        return TimeSpan{QDateTime{startDate}.addDays(1).toTime_t(),
                        QDateTime{endDate}.addDays(1).toTime_t()};
    }
};

// std::ostream& operator<<(std::ostream& os, const DateInterval& timeSpan)
// {
//     TimeSpan t = timeSpan.toTimeSpan();
//     os << "DateInterval: " << t.startTime.day() << "." << t.startTime.month()
//        << "." << t.startTime.year() << " - " << t.finishTime.day() << "."
//        << t.finishTime.month() << "." << t.finishTime.year();
//     return os;
// }

#endif /* end of include guard: DATEINTERVAL_H_DO5P2YKX */
