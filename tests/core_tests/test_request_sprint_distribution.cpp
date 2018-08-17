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

#include "mocks/SprintDistributionReaderMock.h"
#include "gtest/gtest.h"
#include <core/QueryExecutor.h>
#include <core/use_cases/RequestSprintDistribution.h>

using sprint_timer::use_cases::RequestSprintDistribution;
using namespace dw;
using ::testing::_;

// TODO remove when DateWrapper implements equality
namespace dw {

inline bool operator==(const TimeSpan& lhs, const TimeSpan& rhs)
{
    return lhs.start() == rhs.start() && lhs.finish() == rhs.finish();
}

} // namespace dw

class RequestSprintDistributionFixture : public ::testing::Test {
public:
    const TimeSpan someTimeSpan
        = TimeSpan{DateTime::currentDateTime().add(DateTime::Days(-1)),
                   DateTime::currentDateTime().add(DateTime::Days(-1))};
    sprint_timer::QueryExecutor queryExecutor;
    SprintDistributionReaderMock sprint_distribution_reader_mock;
};

TEST_F(RequestSprintDistributionFixture, execute)
{
    EXPECT_CALL(sprint_distribution_reader_mock,
                requestDistribution(someTimeSpan, _))
        .Times(1);

    queryExecutor.executeQuery(std::make_unique<RequestSprintDistribution>(
        sprint_distribution_reader_mock, someTimeSpan, [](const auto& result) {
        }));
}
