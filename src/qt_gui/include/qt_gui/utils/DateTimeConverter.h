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
#ifndef DATETIMECONVERTER_H_Y4Z1XDHQ
#define DATETIMECONVERTER_H_Y4Z1XDHQ

#include "cpp_utils/patterns/Converter.h"
#include "date_wrapper/date_wrapper.h"
#include <QDateTime>

namespace sprint_timer::ui::qt_gui::utils {

class DateTimeConverter : public patterns::Converter<QDateTime, dw::DateTime> {
    [[nodiscard]] auto make_entity_impl(const QDateTime& qDateTime) const
        -> dw::DateTime override;

    [[nodiscard]] auto make_dto_impl(const dw::DateTime& dateTime) const
        -> QDateTime override;
};

class DateConverter : public patterns::Converter<QDate, dw::Date> {
    [[nodiscard]] auto make_entity_impl(const QDate& qDate) const
        -> dw::Date override;

    [[nodiscard]] auto make_dto_impl(const dw::Date& date) const
        -> QDate override;
};

} // namespace sprint_timer::ui::qt_gui::utils

#endif /* end of include guard: DATETIMECONVERTER_H_Y4Z1XDHQ */
