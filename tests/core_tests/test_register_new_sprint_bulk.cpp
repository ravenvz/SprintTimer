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

#include "common_utils/FakeUuidGenerator.h"
#include "core/ObservableActionInvoker.h"
#include "core/actions/RegisterSprintBulk.h"
#include "mocks/SprintStorageMock.h"
#include "gtest/gtest.h"

using sprint_timer::actions::RegisterSprintBulk;
using sprint_timer::entities::Sprint;
using namespace dw;

class RegisterSprintBulkFixture : public ::testing::Test {
public:
    sprint_timer::ObservableActionInvoker actionInvoker;
    mocks::SprintStorageMock sprint_storage_mock;
};

TEST_F(RegisterSprintBulkFixture, execute_and_undo)
{
    using namespace std::chrono_literals;
    using sprint_timer::entities::Tag;

    const std::string taskUuid{"123"};
    const std::list<Tag> tags{Tag{"Tag1"}, Tag{"Tag2"}};
    const DateTimeRange timeRange{current_date_time(),
                                  current_date_time() + 25min};
    const std::string taskName{"Some task name"};
    FakeUuidGenerator generator;
    std::vector<Sprint> sprintBulk{
        Sprint{taskName,
               add_offset(timeRange, 1h),
               tags,
               generator.generateUUID(),
               taskUuid},
        Sprint{taskName,
               add_offset(timeRange, 3h),
               tags,
               generator.generateUUID(),
               taskUuid},
    };

    EXPECT_CALL(sprint_storage_mock, save(sprintBulk)).Times(1);

    actionInvoker.execute(
        std::make_unique<RegisterSprintBulk>(sprint_storage_mock, sprintBulk));

    EXPECT_CALL(sprint_storage_mock, remove(sprintBulk)).Times(1);

    actionInvoker.undo();
}
