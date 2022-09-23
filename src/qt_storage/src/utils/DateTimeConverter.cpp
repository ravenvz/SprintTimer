/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#include "qt_storage/utils/DateTimeConverter.h"

namespace sprint_timer::storage::utils {

auto DateTimeConverter::convert(const QDateTime& qDateTime) const
    -> dw::DateTime
{
    return dw::DateTime{std::chrono::system_clock::time_point{
               std::chrono::milliseconds{qDateTime.toMSecsSinceEpoch()}}} +
           std::chrono::seconds{qDateTime.offsetFromUtc()};
}

auto DateTimeConverter::convert(const dw::DateTime& dateTime) const -> QDateTime
{
    return QDateTime::fromMSecsSinceEpoch(
        dw::to_time_point<std::chrono::milliseconds>(dateTime)
            .time_since_epoch()
            .count(),
        Qt::OffsetFromUTC);
}

auto DateConverter::convert(const QDate& qDate) const -> dw::Date
{
    using namespace dw;
    return Date{Year{qDate.year()},
                Month{static_cast<unsigned>(qDate.month())},
                Day{static_cast<unsigned>(qDate.day())}};
}

auto DateConverter::convert(const dw::Date& date) const -> QDate
{
    return {static_cast<int>(date.year()),
            static_cast<int>(static_cast<unsigned>(date.month())),
            static_cast<int>(static_cast<unsigned>(date.day()))};
}

} // namespace sprint_timer::storage::utils

