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
#include "gtest/gtest.h"

#include "core/HandlerException.h"
#include "use_cases_tests/QtStorageInitializer.h"
#include "gmock/gmock.h"

using namespace sprint_timer;
using namespace sprint_timer::use_cases;
using namespace sprint_timer::entities;
using namespace sprint_timer::compose;
using namespace dw;

class RemovingTaskFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandComposer.createTaskHandler()};
    QueryHandler<ActiveTasksQuery>& activeTasksHandler{
        queryComposer.activeTasksHandler()};
    CommandHandler<DeleteTaskCommand>& deleteTaskHandler{
        commandComposer.deleteTaskHandler()};
    QueryHandler<AllTagsQuery>& allTagsHandler{queryComposer.allTagsHandler()};
    CommandHandler<RegisterSprintBulkCommand>& registerSprintBulkHandler{
        commandComposer.registerSprintBulkHandler()};
    QueryHandler<RequestSprintsQuery>& requestSprintsHandler{
        queryComposer.requestSprintsHandler()};
    TaskDTO task{"",
                 {"Tag1", "Tag2"},
                 "Task name",
                 4,
                 0,
                 false,
                 dw::current_date_time()};
};

TEST_F(RemovingTaskFixture, removing_task_with_no_sprints)
{
    createTaskHandler.handle(
        CreateTaskCommand{task.name, task.tags, task.expectedCost});
    const auto uuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;

    deleteTaskHandler.handle(DeleteTaskCommand{uuid});

    EXPECT_TRUE(activeTasksHandler.handle(ActiveTasksQuery{}).empty());
    EXPECT_TRUE(allTagsHandler.handle(AllTagsQuery{}).empty());
}

TEST_F(RemovingTaskFixture, removing_task_that_has_assosiated_sprints)
{
    using namespace std::chrono_literals;
    createTaskHandler.handle(
        CreateTaskCommand{task.name, task.tags, task.expectedCost});
    const dw::Date someDate{Year{2021}, Month{3}, Day{3}};
    const dw::DateTime firstSprintStartTime{DateTime{someDate} + 3h};
    const dw::DateTimeRange firstSprintRange{firstSprintStartTime,
                                             firstSprintStartTime + 25min};
    const std::vector<DateTimeRange> intervals{
        firstSprintRange,
        add_offset(firstSprintRange, 1h + 25min),
        add_offset(firstSprintRange, 5h + 50min),
        add_offset(firstSprintRange, 7h + 75min)};
    const auto uuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    registerSprintBulkHandler.handle(
        RegisterSprintBulkCommand(uuid, intervals));

    deleteTaskHandler.handle(DeleteTaskCommand{uuid});

    EXPECT_TRUE(activeTasksHandler.handle(ActiveTasksQuery{}).empty());
    EXPECT_TRUE(allTagsHandler.handle(AllTagsQuery{}).empty());
    EXPECT_TRUE(
        requestSprintsHandler
            .handle(RequestSprintsQuery{dw::DateRange{someDate, someDate}})
            .empty());
}

TEST_F(RemovingTaskFixture, removing_task_cleanes_up_orphaned_tags)
{
    createTaskHandler.handle(
        CreateTaskCommand{task.name, task.tags, task.expectedCost});
    const auto uuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;

    deleteTaskHandler.handle(DeleteTaskCommand{uuid});

    EXPECT_TRUE(allTagsHandler.handle(AllTagsQuery{}).empty());
}

TEST_F(RemovingTaskFixture,
       removing_task_does_not_remove_tag_if_some_other_task_has_it)
{
    createTaskHandler.handle(
        CreateTaskCommand{"Some task", {"Tag1", "SharedTag"}, 7});
    createTaskHandler.handle(
        CreateTaskCommand{"Some other task", {"SharedTag", "Tag2"}, 7});
    const auto tasks = activeTasksHandler.handle(ActiveTasksQuery{});

    deleteTaskHandler.handle(DeleteTaskCommand{tasks.front().uuid});

    EXPECT_THAT(allTagsHandler.handle(AllTagsQuery{}),
                ::testing::ElementsAre("SharedTag", "Tag2"));
}

TEST_F(RemovingTaskFixture,
       throws_when_trying_to_remove_task_that_does_not_exist)
{
    EXPECT_THROW(
        deleteTaskHandler.handle(DeleteTaskCommand{"Non-existing uuid"}),
        HandlerException);
}
