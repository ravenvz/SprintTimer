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
#ifndef DATETIMEPROVIDERMOCK_H_JFCR0B6M
#define DATETIMEPROVIDERMOCK_H_JFCR0B6M

#include "core/DateTimeProvider.h"
#include "gmock/gmock.h"

namespace mocks {

class DateTimeProviderMock : public sprint_timer::DateTimeProvider {
public:
    MOCK_METHOD(dw::Date, dateNow, (), (override));

    MOCK_METHOD(dw::DateTime, dateTimeNow, (), (override));

    MOCK_METHOD(dw::Date, dateLocalNow, (), (override));

    MOCK_METHOD(dw::DateTime, dateTimeLocalNow, (), (override));
};

} // namespace mocks

#endif /* end of include guard: DATETIMEPROVIDERMOCK_H_JFCR0B6M */
