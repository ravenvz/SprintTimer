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
#include "use_cases_tests/QtStorageInitializer.h"
#include "gtest/gtest.h"

using namespace sprint_timer;
using namespace sprint_timer::use_cases;
using namespace sprint_timer::entities;
using namespace sprint_timer::compose;
using namespace dw;

class RegisteringSprintsFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    QueryHandler<ActiveTasksQuery>& activeTasksHandler{
        queryComposer.activeTasksHandler()};
    CommandHandler<RegisterSprintBulkCommand>& registerSprintsHandler{
        commandComposer.registerSprintBulkHandler()};
    CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandComposer.createTaskHandler()};
};

TEST_F(RegisteringSprintsFixture,
       registering_sprints_increments_task_actual_count)
{
    using namespace std::chrono_literals;
    createTaskHandler.handle(CreateTaskCommand{"Some task", {"Tag1"}, 5});
    const auto taskUuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    const DateTimeRange range{current_date_time_local(),
                              current_date_time_local() + 25min};
    const std::vector<dw::DateTimeRange> intervals{range,
                                                   add_offset(range, 3h)};
    const TaskDTO expected{taskUuid,
                           {"Tag1"},
                           "Some task",
                           5,
                           2,
                           false,
                           current_date_time_local()};

    registerSprintsHandler.handle(
        RegisterSprintBulkCommand{taskUuid, intervals});

    const auto activeTasks = activeTasksHandler.handle(ActiveTasksQuery{});
    EXPECT_EQ(expected, activeTasks.front());
}
