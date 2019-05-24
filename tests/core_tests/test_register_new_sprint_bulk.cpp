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

#include "mocks/SprintStorageWriterMock.h"
#include "gtest/gtest.h"
#include <algorithm>
#include <core/CommandInvoker.h>
#include <core/SprintBuilder.h>
#include <core/use_cases/RegisterNewSprintBulk.h>

using sprint_timer::entities::Sprint;
using sprint_timer::use_cases::RegisterNewSprintBulk;
using namespace dw;

namespace {

std::vector<sprint_timer::entities::Sprint>
generateSomeSprints(size_t numSprints)
{
    using namespace sprint_timer;
    using namespace std::chrono;
    using namespace std::chrono_literals;
    std::vector<entities::Sprint> sprints;
    SprintBuilder builder;
    const dw::DateTime start{current_date_time()};
    int i{0};
    auto gen = [&start, &i, &builder]() {
        const DateTimeRange time_range{
            add_offset(DateTimeRange{start, start + 25min}, hours{i})};
        return builder.withTaskUuid(std::to_string(i++))
            .withTimeSpan(time_range)
            .build();
    };
    std::generate_n(std::back_inserter(sprints), numSprints, gen);
    return sprints;
}

} // namespace

class RegisterNewSprintBulkFixture : public ::testing::Test {
public:
    sprint_timer::CommandInvoker commandInvoker;
    SprintStorageWriterMock sprint_writer_mock;
    const std::vector<Sprint> sprintBulk = generateSomeSprints(5);
};

TEST_F(RegisterNewSprintBulkFixture, execute_and_undo)
{
    EXPECT_CALL(sprint_writer_mock, save(sprintBulk)).Times(1);

    commandInvoker.executeCommand(std::make_unique<RegisterNewSprintBulk>(
        sprint_writer_mock, sprintBulk));

    EXPECT_CALL(sprint_writer_mock, remove(sprintBulk)).Times(1);

    commandInvoker.undo();
}
