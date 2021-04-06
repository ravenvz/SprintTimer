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
#include "mocks/OperationalRangeReaderMock.h"
#include "gtest/gtest.h"
#include <core/use_cases/request_op_range/OperationalRangeHandler.h>
#include <core/use_cases/request_op_range/OperationalRangeQuery.h>

using sprint_timer::use_cases::OperationalRangeHandler;
using sprint_timer::use_cases::OperationalRangeQuery;
using ::testing::_;

class RequestOperationalRangeFixture : public ::testing::Test {
public:
    mocks::OperationalRangeReaderMock year_range_reader_mock;
    OperationalRangeHandler handler{year_range_reader_mock};
};

TEST_F(RequestOperationalRangeFixture, handle)
{
    ON_CALL(year_range_reader_mock, operationalRange())
        .WillByDefault(::testing::Return(
            dw::DateRange{dw::current_date(), dw::current_date()}));
    EXPECT_CALL(year_range_reader_mock, operationalRange()).Times(1);

    handler.handle(OperationalRangeQuery{});
}
