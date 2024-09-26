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
#ifndef DATETIMEPROVIDER_H_ND1KPYZU
#define DATETIMEPROVIDER_H_ND1KPYZU

#include "date_wrapper/date_wrapper.h"

namespace sprint_timer::api {

class DateTimeProvider {
public:
    virtual ~DateTimeProvider() = default;

    [[nodiscard]] auto dateNow() const -> dw::Date { return dateNowImpl(); }

    [[nodiscard]] auto dateTimeNow() const -> dw::DateTime
    {
        return dateTimeNowImpl();
    }

    [[nodiscard]] auto dateLocalNow() const -> dw::Date
    {
        return dateLocalNowImpl();
    }

    [[nodiscard]] auto dateTimeLocalNow() const -> dw::DateTime
    {
        return dateTimeLocalNowImpl();
    }

private:
    virtual auto dateNowImpl() const -> dw::Date = 0;

    virtual auto dateTimeNowImpl() const -> dw::DateTime = 0;

    virtual auto dateLocalNowImpl() const -> dw::Date = 0;

    virtual auto dateTimeLocalNowImpl() const -> dw::DateTime = 0;
};

} // namespace sprint_timer::api

#endif /* end of include guard: DATETIMEPROVIDER_H_ND1KPYZU */
