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
using namespace dw;

class RemovingSprintsFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    compose::CommandHandlerComposer& commandHandlerComposer{
        initializer.commandHandlerComposer()};
    compose::QueryHandlerComposer& queryHandlerComposer{
        initializer.queryHandlerComposer()};
    QueryHandler<ActiveTasksQuery>& activeTasksHandler{
        queryHandlerComposer.activeTasksHandler()};
    CommandHandler<RegisterSprintBulkCommand>& registerSprintsHandler{
        commandHandlerComposer.registerSprintBulkHandler()};
    CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandHandlerComposer.createTaskHandler()};
    QueryHandler<SprintsForTaskQuery>& sprintsForTaskHandler{
        queryHandlerComposer.sprintsForTaskHandler()};
    CommandHandler<DeleteSprintCommand>& deleteSprintHandler{
        commandHandlerComposer.deleteSprintHandler()};
};

TEST_F(RemovingSprintsFixture, removing_sprints_decrements_task_actual_count)
{
    using namespace std::chrono_literals;
    createTaskHandler.handle(CreateTaskCommand{"Some task", {"Tag1"}, 5});
    const auto taskUuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    const DateTimeRange range{current_date_time_local(),
                              current_date_time_local() + 25min};
    const std::vector<dw::DateTimeRange> intervals{
        range, add_offset(range, 3h), add_offset(range, 5h)};
    registerSprintsHandler.handle(
        RegisterSprintBulkCommand{taskUuid, intervals});
    const auto sprints =
        sprintsForTaskHandler.handle(SprintsForTaskQuery{taskUuid});

    deleteSprintHandler.handle(DeleteSprintCommand{sprints.front().uuid});

    EXPECT_EQ(2,
              activeTasksHandler.handle(ActiveTasksQuery{}).front().actualCost);
}
