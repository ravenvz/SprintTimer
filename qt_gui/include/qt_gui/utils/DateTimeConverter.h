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
#ifndef DATETIMECONVERTER_H_Y4Z1XDHQ
#define DATETIMECONVERTER_H_Y4Z1XDHQ

#include <QDateTime>
#include <date_wrapper/date_wrapper.h>

namespace sprint_timer::ui::qt_gui {

using dw::DateTime;
using dw::DateTimeRange;

// TODO no real need to have a class here

/* Simplifies convertion between QDateTime and DateTime. */
class DateTimeConverter {
public:
    static QDateTime qDateTime(const DateTime& dt)
    {
        return QDateTime::fromMSecsSinceEpoch(
            dw::to_time_point<std::chrono::milliseconds>(dt)
                .time_since_epoch()
                .count(),
            Qt::OffsetFromUTC);
        // return QDateTime::fromTime_t(static_cast<unsigned>(dt.toTime_t()),
        // Qt::OffsetFromUTC);
    }

    static QDate qDate(const DateTime& dt) { return qDateTime(dt).date(); }

    static DateTime dateTime(const QDateTime& qdt)
    {
        return DateTime{std::chrono::system_clock::time_point{
                   std::chrono::milliseconds{qdt.toMSecsSinceEpoch()}}}
        + std::chrono::seconds{qdt.offsetFromUtc()};
        // return DateTime::fromTime_t(qdt.toTime_t(), qdt.offsetFromUtc());
    }

    static dw::Date date(const QDate& qdate)
    {
        using namespace dw;
        return Date{Year{qdate.year()},
                    Month{static_cast<unsigned>(qdate.month())},
                    Day{static_cast<unsigned>(qdate.day())}};
    }
};

} // namespace sprint_timer::ui::qt_gui


#endif /* end of include guard: DATETIMECONVERTER_H_Y4Z1XDHQ */
