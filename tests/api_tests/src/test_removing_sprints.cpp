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
#include "api_tests/QtStorageInitializer.h"
#include "api_tests/matchers/MatchesTaskIgnoringUuid.h"
#include "gtest/gtest.h"

using namespace sprint_timer;
using namespace sprint_timer::api;
using namespace sprint_timer::entities;
using namespace dw;

class RemovingSprintsFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    compose::CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    compose::QueryHandlerComposer& queryHandlerComposer{
        initializer.queryHandlerComposer()};
    asp::QueryHandler<ActiveTasksQuery>& activeTasksHandler{
        queryHandlerComposer.activeTasksHandler()};
    asp::CommandHandler<RegisterSprintBulkCommand>& registerSprintsHandler{
        commandComposer.registerSprintBulkHandler()};
    asp::CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandComposer.createTaskHandler()};
    asp::QueryHandler<SprintsForTaskQuery>& sprintsForTaskHandler{
        queryHandlerComposer.sprintsForTaskHandler()};
    asp::CommandHandler<DeleteSprintCommand>& deleteSprintHandler{
        commandComposer.deleteSprintHandler()};
    asp::CommandHandler<UndoLastCommand>& undoHandler{
        commandComposer.undoHandler()};
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

    deleteSprintHandler.handle(DeleteSprintCommand{add_offset(range, 3h)});
    const auto task = activeTasksHandler.handle(ActiveTasksQuery{}).front();

    EXPECT_THAT(task,
                ::testing::Truly(matchers::MatchesTaskIgnoringUuid(
                    TaskDTO{"irrelevant_uuid",
                            {"Tag1"},
                            "Some task",
                            5,
                            {range, add_offset(range, 5h)},
                            false,
                            current_date_time_local()})));
}

TEST_F(RemovingSprintsFixture, undoing_removing_sprints)
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
    deleteSprintHandler.handle(DeleteSprintCommand{add_offset(range, 3h)});

    undoHandler.handle(UndoLastCommand{});
    const auto task = activeTasksHandler.handle(ActiveTasksQuery{}).front();

    EXPECT_THAT(task,
                ::testing::Truly(matchers::MatchesTaskIgnoringUuid(TaskDTO{
                    "irrelevant_uuid",
                    {"Tag1"},
                    "Some task",
                    5,
                    {range, add_offset(range, 3h), add_offset(range, 5h)},
                    false,
                    current_date_time_local()})));
}
