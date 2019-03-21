/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#ifndef DATEINTERVAL_H_DO5P2YKX
#define DATEINTERVAL_H_DO5P2YKX

#include "qt_gui/utils/DateTimeConverter.h"
#include <QDate>
#include <date_wrapper/date_wrapper.h>
#include <iostream>

namespace sprint_timer::ui::qt_gui {

struct DateInterval {

    QDate startDate;
    QDate endDate;

    static DateInterval fromTimeSpan(const dw::DateTimeRange& timeSpan)
    {
        return DateInterval{DateTimeConverter::qDate(timeSpan.start()),
                            DateTimeConverter::qDate(timeSpan.finish())};
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

    dw::DateTimeRange toTimeSpan() const
    {
        return dw::DateTimeRange{
            DateTimeConverter::dateTime(QDateTime{startDate}),
            DateTimeConverter::dateTime(QDateTime{endDate})};
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

} // namespace sprint_timer::ui::qt_gui


#endif /* end of include guard: DATEINTERVAL_H_DO5P2YKX */
