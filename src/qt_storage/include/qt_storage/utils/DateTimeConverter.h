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
#ifndef DATETIMECONVERTER_H_Y9WAS2OJ
#define DATETIMECONVERTER_H_Y9WAS2OJ

#include "core/utils/Converter.h"
#include "date_wrapper/date_wrapper.h"
#include <QDateTime>

namespace sprint_timer::storage::utils {

class DateTimeConverter : public Converter<QDateTime, dw::DateTime> {
    [[nodiscard]] auto convert(const QDateTime& qDateTime) const
        -> dw::DateTime override;

    [[nodiscard]] auto convert(const dw::DateTime& dateTime) const
        -> QDateTime override;
};

class DateConverter : public Converter<QDate, dw::Date> {
    [[nodiscard]] auto convert(const QDate& qDate) const -> dw::Date override;

    [[nodiscard]] auto convert(const dw::Date& date) const -> QDate override;
};

} // namespace sprint_timer::storage::utils

#endif /* end of include guard: DATETIMECONVERTER_H_Y9WAS2OJ */

