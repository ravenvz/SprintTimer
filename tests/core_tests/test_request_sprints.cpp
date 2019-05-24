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

#include "mocks/SprintStorageReaderMock.h"
#include "gtest/gtest.h"
#include <core/QueryInvoker.h>
#include <core/use_cases/RequestSprints.h>

using namespace dw;
using sprint_timer::QueryInvoker;
using sprint_timer::entities::Sprint;
using sprint_timer::use_cases::RequestSprints;
using ::testing::_;

class RequestSprintsFixture : public ::testing::Test {
public:
    SprintStorageReaderMock sprint_reader_mock;
    QueryInvoker query_executor;

    const DateRange someDateRange{
        dw::add_offset({current_date_local(), current_date_local()}, Days{-1})};
};

TEST_F(RequestSprintsFixture, execute)
{
    EXPECT_CALL(sprint_reader_mock, requestItems(someDateRange, _));

    query_executor.execute(std::make_unique<RequestSprints>(
        sprint_reader_mock, someDateRange, [](const auto& result) {}));
}
