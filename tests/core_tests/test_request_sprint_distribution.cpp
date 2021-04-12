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

#include "mocks/SprintDistributionReaderMock.h"
#include "gtest/gtest.h"
#include "core/use_cases/request_sprint_distribution/RequestSprintDistributionHandler.h"

using sprint_timer::use_cases::RequestSprintDistributionHandler;
using sprint_timer::use_cases::RequestSprintDistributionQuery;
using namespace dw;
using ::testing::_;

class RequestSprintDistributionFixture : public ::testing::Test {
public:
    const DateRange someDateRange{
        add_offset({current_date(), current_date()}, Days{-1})};
    mocks::SprintDistributionReaderMock sprint_distribution_reader_mock;
    RequestSprintDistributionHandler handler{sprint_distribution_reader_mock};
};

TEST_F(RequestSprintDistributionFixture, execute)
{
    EXPECT_CALL(sprint_distribution_reader_mock,
                sprintDistribution(someDateRange))
        .Times(1);

    auto dateRange = someDateRange;
    handler.handle(RequestSprintDistributionQuery{std::move(dateRange)});
}
