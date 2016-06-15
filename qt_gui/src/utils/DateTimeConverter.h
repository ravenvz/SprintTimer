#ifndef DATETIMECONVERTER_H_Y4Z1XDHQ
#define DATETIMECONVERTER_H_Y4Z1XDHQ

#include "core/DateTime.h"
#include <QDateTime>

/* Simplifies convertion between QDateTime and DateTime. */
class DateTimeConverter {
public:
    static QDateTime qDateTime(const DateTime& dt)
    {
        return QDateTime::fromTime_t(static_cast<unsigned>(dt.toTime_t()),
                                     Qt::OffsetFromUTC);
    }

    static QDate qDate(const DateTime& dt) { return qDateTime(dt).date(); }

    static DateTime dateTime(const QDateTime& qdt)
    {
        return DateTime::fromTime_t(qdt.toTime_t(), qdt.offsetFromUtc());
    }
};

#endif /* end of include guard: DATETIMECONVERTER_H_Y4Z1XDHQ */
