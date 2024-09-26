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

#include "api/DateTimeProvider.h"

namespace sprint_timer::api {

class DefaultDateTimeProvider : public DateTimeProvider {
private:
    auto dateNowImpl() const -> dw::Date override;

    auto dateTimeNowImpl() const -> dw::DateTime override;

    auto dateLocalNowImpl() const -> dw::Date override;

    auto dateTimeLocalNowImpl() const -> dw::DateTime override;
};

inline auto DefaultDateTimeProvider::dateNowImpl() const -> dw::Date
{
    return dw::current_date();
}

inline auto DefaultDateTimeProvider::dateTimeNowImpl() const -> dw::DateTime
{
    return dw::current_date_time();
}

inline auto DefaultDateTimeProvider::dateLocalNowImpl() const -> dw::Date
{
    return dw::current_date_local();
}

inline auto
DefaultDateTimeProvider::dateTimeLocalNowImpl() const -> dw::DateTime
{
    return dw::current_date_time_local();
}

} // namespace sprint_timer::api

#endif /* end of include guard: DEFAULTDATETIMEPROVIDER_H_WN1DU3KC */
