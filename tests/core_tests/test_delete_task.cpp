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

#include "core/ActionInvoker.h"
#include "core/ObservableActionInvoker.h"
#include "core/actions/DeleteTask.h"
#include "mocks/SprintStorageMock.h"
#include "mocks/TaskStorageMock.h"
#include "gtest/gtest.h"

#include <thread>

using ::testing::_;
using ::testing::InvokeArgument;
using namespace std::chrono_literals;

using sprint_timer::actions::DeleteTask;
using sprint_timer::entities::Sprint;
using sprint_timer::entities::Tag;
using sprint_timer::entities::Task;

using namespace dw;

class DeleteTaskFixture : public ::testing::Test {
public:
    testing::NiceMock<mocks::TaskStorageMock> task_storage_mock;
    sprint_timer::ObservableActionInvoker actionInvoker;

    std::string someTaskUuid{"550e8400-e29b-41d4-a716-446655440000"};
    dw::DateTimeRange someTimeSpan{dw::current_date_time(),
                                   dw::current_date_time() + 25min};
    Task taskWithSprints{"Task name",
                         4,
                         {Sprint{"Task name",
                                 someTimeSpan,
                                 {Tag{"Tag1"}, Tag{"Tag2"}},
                                 "1",
                                 someTaskUuid},
                          Sprint{"Task name",
                                 dw::add_offset(someTimeSpan, 25min),
                                 {Tag{"Tag1"}, Tag{"Tag2"}},
                                 "2",
                                 someTaskUuid}},
                         "550e8400-e29b-41d4-a716-446655440000",
                         {Tag{"Tag1"}, Tag{"Tag2"}},
                         false,
                         DateTime{Date{Year{2015}, Month{11}, Day{10}}}};
};

TEST_F(DeleteTaskFixture, delete_task_with_sprints)
{
    EXPECT_CALL(task_storage_mock, remove(taskWithSprints.uuid())).Times(1);

    actionInvoker.execute(
        std::make_unique<DeleteTask>(task_storage_mock, taskWithSprints));
}

TEST_F(DeleteTaskFixture, undo_deletion_of_task_with_sprints)
{
    EXPECT_CALL(task_storage_mock, remove(taskWithSprints.uuid())).Times(1);

    actionInvoker.execute(
        std::make_unique<DeleteTask>(task_storage_mock, taskWithSprints));

    EXPECT_CALL(task_storage_mock, save(taskWithSprints)).Times(1);

    actionInvoker.undo();
}
