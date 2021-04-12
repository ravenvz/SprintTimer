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

#include "mocks/SprintStorageMock.h"
#include "gtest/gtest.h"
#include <algorithm>
#include "core/ObservableActionInvoker.h"
#include "core/SprintBuilder.h"
#include "core/actions/RegisterSprintBulk.h"

using sprint_timer::actions::RegisterSprintBulk;
using sprint_timer::entities::Sprint;
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

class RegisterSprintBulkFixture : public ::testing::Test {
public:
    sprint_timer::ObservableActionInvoker actionInvoker;
    mocks::SprintStorageMock sprint_storage_mock;
    const std::vector<Sprint> sprintBulk = generateSomeSprints(5);
};

TEST_F(RegisterSprintBulkFixture, execute_and_undo)
{
    EXPECT_CALL(sprint_storage_mock, save(sprintBulk)).Times(1);

    actionInvoker.execute(
        std::make_unique<RegisterSprintBulk>(sprint_storage_mock, sprintBulk));

    EXPECT_CALL(sprint_storage_mock, remove(sprintBulk)).Times(1);

    actionInvoker.undo();
}
