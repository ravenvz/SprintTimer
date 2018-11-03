/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

// TODO remove when Gtest drops std::tr1
// Workaround for C++17 as std::tr1 no longer available and Gtest uses it
#define GTEST_LANG_CXX11 1

#include "mocks/YearRangeReaderMock.h"
#include "gtest/gtest.h"
#include <core/QueryInvoker.h>
#include <core/use_cases/RequestMinMaxYear.h>

using sprint_timer::use_cases::RequestMinMaxYear;
using ::testing::_;

class RequestMinMaxYearFixture : public ::testing::Test {
public:
    sprint_timer::QueryInvoker queryInvoker;
    YearRangeReaderMock year_range_reader_mock;
};

TEST_F(RequestMinMaxYearFixture, execute)
{
    EXPECT_CALL(year_range_reader_mock, requestYearRange(_)).Times(1);

    queryInvoker.execute(std::make_unique<RequestMinMaxYear>(
        year_range_reader_mock, [](const auto& result) {}));
}
