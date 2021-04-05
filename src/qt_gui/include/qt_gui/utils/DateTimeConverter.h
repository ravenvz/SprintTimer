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
#ifndef DATETIMECONVERTER_H_Y4Z1XDHQ
#define DATETIMECONVERTER_H_Y4Z1XDHQ

#include <QDateTime>
#include <date_wrapper/date_wrapper.h>

namespace sprint_timer::ui::qt_gui::utils {

QDateTime toQDateTime(const dw::DateTime& dt);

QDate toQDate(const dw::DateTime& dt);

QDate toQDate(const dw::Date& date);

dw::DateTime toDateTime(const QDateTime& qdt);

dw::Date toDate(const QDate& date);

} // namespace sprint_timer::ui::qt_gui::utils

#endif /* end of include guard: DATETIMECONVERTER_H_Y4Z1XDHQ */
