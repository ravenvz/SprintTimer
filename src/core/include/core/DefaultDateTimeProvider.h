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
#ifndef DEFAULTDATETIMEPROVIDER_H_WN1DU3KC
#define DEFAULTDATETIMEPROVIDER_H_WN1DU3KC

#include "core/DateTimeProvider.h"

namespace sprint_timer {

class DefaultDateTimeProvider : public DateTimeProvider {
public:
    dw::Date dateNow() override;

    dw::DateTime dateTimeNow() override;

    dw::Date dateLocalNow() override;

    dw::DateTime dateTimeLocalNow() override;
};

inline dw::Date DefaultDateTimeProvider::dateNow()
{
    return dw::current_date();
}

inline dw::DateTime DefaultDateTimeProvider::dateTimeNow()
{
    return dw::current_date_time();
}

inline dw::Date DefaultDateTimeProvider::dateLocalNow()
{
    return dw::current_date_local();
}

inline dw::DateTime DefaultDateTimeProvider::dateTimeLocalNow()
{
    return dw::current_date_time_local();
}

} // namespace sprint_timer

#endif /* end of include guard: DEFAULTDATETIMEPROVIDER_H_WN1DU3KC */
