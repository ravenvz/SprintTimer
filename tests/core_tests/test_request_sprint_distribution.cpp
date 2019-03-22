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

#include "mocks/SprintDistributionReaderMock.h"
#include "gtest/gtest.h"
#include <core/QueryInvoker.h>
#include <core/use_cases/RequestSprintDistribution.h>

using sprint_timer::use_cases::RequestSprintDistribution;
using namespace dw;
using ::testing::_;

class RequestSprintDistributionFixture : public ::testing::Test {
public:
    const DateRange someDateRange{
        add_offset({current_date(), current_date()}, Days{-1})};
    sprint_timer::QueryInvoker queryInvoker;
    SprintDistributionReaderMock sprint_distribution_reader_mock;
};

TEST_F(RequestSprintDistributionFixture, execute)
{
    EXPECT_CALL(sprint_distribution_reader_mock,
                requestDistribution(someDateRange, _))
        .Times(1);

    queryInvoker.execute(std::make_unique<RequestSprintDistribution>(
        sprint_distribution_reader_mock, someDateRange, [](const auto& result) {
        }));
}
