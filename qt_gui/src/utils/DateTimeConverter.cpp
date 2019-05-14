/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#include "qt_gui/utils/DateTimeConverter.h"

namespace sprint_timer::ui::qt_gui::utils {

QDateTime toQDateTime(const dw::DateTime& dt)
{
    return QDateTime::fromMSecsSinceEpoch(
        dw::to_time_point<std::chrono::milliseconds>(dt)
            .time_since_epoch()
            .count(),
        Qt::OffsetFromUTC);
}


QDate toQDate(const dw::DateTime& dt) { return toQDateTime(dt).date(); }


QDate toQDate(const dw::Date& date)
{
    return QDate(static_cast<int>(date.year()),
                 static_cast<unsigned>(date.month()),
                 static_cast<unsigned>(date.day()));
}


dw::DateTime toDateTime(const QDateTime& qdt)
{
    return DateTime{std::chrono::system_clock::time_point{
               std::chrono::milliseconds{qdt.toMSecsSinceEpoch()}}}
    + std::chrono::seconds{qdt.offsetFromUtc()};
}


dw::Date toDate(const QDate& date)
{
    using namespace dw;
    return Date{Year{date.year()},
                Month{static_cast<unsigned>(date.month())},
                Day{static_cast<unsigned>(date.day())}};
}

} // namespace sprint_timer::ui::qt_gui::utils
