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
#include "api/HandlerException.h"
#include "api_tests/QtStorageInitializer.h"
#include "api_tests/fixtures/TaskTreeFixture.h"
#include "gmock/gmock.h"

using namespace sprint_timer;
using namespace sprint_timer::api;
using namespace sprint_timer;
using namespace sprint_timer::compose;
using namespace dw;

class RemovingTaskFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    asp::CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandComposer.createTaskHandler()};
    asp::QueryHandler<ActiveTasksQuery>& activeTasksHandler{
        queryComposer.activeTasksHandler()};
    asp::CommandHandler<DeleteTaskCommand>& deleteTaskHandler{
        commandComposer.deleteTaskHandler()};
    asp::QueryHandler<AllTagsQuery>& allTagsHandler{
        queryComposer.allTagsHandler()};
    asp::CommandHandler<RegisterSprintBulkCommand>& registerSprintBulkHandler{
        commandComposer.registerSprintBulkHandler()};
    asp::QueryHandler<RequestSprintsQuery>& requestSprintsHandler{
        queryComposer.requestSprintsHandler()};
    asp::QueryHandler<SprintsForTaskQuery>& sprintsForTaskHandler{
        queryComposer.sprintsForTaskHandler()};
    asp::QueryHandler<ReadTaskTreeQuery>& readTaskTreeHandler{
        queryComposer.readTaskTreeHandler()};
    asp::CommandHandler<UndoLastCommand>& undoHandler{
        commandComposer.undoHandler()};
    TaskDTO task{"",
                 {"Tag1", "Tag2"},
                 "Task name",
                 4,
                 {},
                 false,
                 dw::current_date_time(),
                 std::nullopt,
                 TaskTimeframeDTO{},
                 TaskTypeDTO::Regular};
};

/*    Task tree fixture used in these tests looks like that
 *
 *    0 folder1
 *    1    project1
 *    2       task1
 *    5       task3 (3 sprints)
 *    3       project2
 *    4 task2
 *
 *
 */

auto uuid_projection = [](const auto& node) { return node.uuid; };

TEST_F(RemovingTaskFixture, removing_task_with_no_sprints)
{
    auto tree = fixtures::givenSomeTaskTreeCreated(createTaskHandler,
                                                   registerSprintBulkHandler);
    tree.erase(std::ranges::find(tree, "3", uuid_projection));

    deleteTaskHandler.handle(DeleteTaskCommand{"3"});

    EXPECT_THAT(
        allTagsHandler.handle(AllTagsQuery{}),
        ::testing::UnorderedElementsAre("Tag1", "Tag2", "Tag3", "ProjectTag1"));
    EXPECT_EQ(tree, readTaskTreeHandler.handle(ReadTaskTreeQuery{}));
}

TEST_F(RemovingTaskFixture, removing_task_that_has_assosiated_sprints)
{
    auto tree = fixtures::givenSomeTaskTreeCreated(createTaskHandler,
                                                   registerSprintBulkHandler);
    tree.erase(std::ranges::find(tree, "5", uuid_projection));

    deleteTaskHandler.handle(DeleteTaskCommand{"5"});

    EXPECT_EQ(tree, readTaskTreeHandler.handle(ReadTaskTreeQuery{}));
    EXPECT_THAT(allTagsHandler.handle(AllTagsQuery{}),
                ::testing::UnorderedElementsAre("Tag1", "Tag2", "ProjectTag1"));
}

TEST_F(RemovingTaskFixture,
       throws_when_trying_to_remove_task_that_does_not_exist)
{
    EXPECT_THROW(
        deleteTaskHandler.handle(DeleteTaskCommand{"Non-existing uuid"}),
        HandlerException);
}

TEST_F(RemovingTaskFixture,
       undoing_removing_parent_task_restores_children_and_tags_and_sprints)
{
    auto tree = fixtures::givenSomeTaskTreeCreated(createTaskHandler,
                                                   registerSprintBulkHandler);
    deleteTaskHandler.handle(DeleteTaskCommand{"1"});

    undoHandler.handle(UndoLastCommand{});

    EXPECT_EQ(tree, readTaskTreeHandler.handle(ReadTaskTreeQuery{}));
    EXPECT_THAT(
        allTagsHandler.handle(AllTagsQuery{}),
        ::testing::UnorderedElementsAre("Tag1", "Tag2", "Tag3", "ProjectTag1"));

    // using namespace std::chrono_literals;
    // createTaskHandler.handle(
    //     CreateTaskCommand{task.name, task.tags, task.expectedCost});
    // const dw::Date someDate{Year{2021}, Month{3}, Day{3}};
    // const dw::DateTime firstSprintStartTime{DateTime{someDate} + 3h};
    // const dw::DateTimeRange firstSprintRange{firstSprintStartTime,
    //                                          firstSprintStartTime + 25min};
    // const std::vector<DateTimeRange> intervals{
    //     firstSprintRange,
    //     add_offset(firstSprintRange, 1h + 25min),
    //     add_offset(firstSprintRange, 5h + 50min),
    //     add_offset(firstSprintRange, 7h + 75min)};
    // const auto storedTask =
    //     activeTasksHandler.handle(ActiveTasksQuery{}).front();
    // const auto uuid = storedTask.uuid;
    // const TaskDTO expected{uuid,
    //                        task.tags,
    //                        task.name,
    //                        task.expectedCost,
    //                        intervals,
    //                        false,
    //                        current_date_time_local()};
    // registerSprintBulkHandler.handle(
    //     RegisterSprintBulkCommand(uuid, intervals));
    // deleteTaskHandler.handle(DeleteTaskCommand{uuid});
    //
    // undoHandler.handle(UndoLastCommand{});
    // const auto actual =
    // activeTasksHandler.handle(ActiveTasksQuery{}).front();
    //
    // EXPECT_EQ(expected, actual);
    // EXPECT_THAT(allTagsHandler.handle(AllTagsQuery{}),
    //             ::testing::ElementsAre("Tag1", "Tag2"));
}
