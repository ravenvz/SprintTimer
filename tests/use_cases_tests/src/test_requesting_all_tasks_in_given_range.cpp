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
#include "use_cases_tests/matchers/MatchesTaskIgnoringUuid.h"
#include "gmock/gmock.h"

using namespace sprint_timer;
using namespace sprint_timer::use_cases;
using namespace sprint_timer::entities;
using namespace sprint_timer::compose;
using namespace dw;

class RequestingFinishedTasksFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandComposer.createTaskHandler()};
    QueryHandler<ActiveTasksQuery>& activeTasksHandler{
        queryComposer.activeTasksHandler()};
    QueryHandler<RequestSprintsQuery>& requestSprintsHandler{
        queryComposer.requestSprintsHandler()};
    QueryHandler<FinishedTasksQuery>& finishedTasksHandler{
        queryComposer.finishedTasksHandler()};
    CommandHandler<ToggleTaskCompletedCommand>& toggleTaskCompletedHandler{
        commandComposer.toggleTaskCompletedHandler()};
};

TEST_F(QtStorageTestFixture, requests_all_tasks_in_given_date_range)
{
    using namespace dw;
    const auto taskStorage = initializer.factory.taskStorage();
    const DateRange targetDateRange{Date{Year{2018}, Month{10}, Day{10}},
                                    Date{Year{2018}, Month{10}, Day{15}}};
    const DateTime leftmostDateTime{DateTime{targetDateRange.start()}};
    const DateTime rightmostDateTime{DateTime{targetDateRange.finish()}};
    TaskBuilder builder;
    const std::string taskName{"Some name"};
    const std::string taskUuid{generator.generateUUID()};
    const std::list<Tag> tags{Tag{"Tag"}, Tag{"OtherTag"}};
    const Task taskOutOfRangeLeft = Task{taskName, 5, 0, taskUuid, tags, false, fakeTimeSource};
        builder.withLastModificationStamp(leftmostDateTime - Days{1}).build();
    const Task taskLeftRangeBorder =
        builder.withLastModificationStamp(leftmostDateTime).build();
    const Task insideRangeTask1 =
        builder.withLastModificationStamp(leftmostDateTime + Days{1}).build();
    const Task insideRangeTask2 =
        builder.withLastModificationStamp(rightmostDateTime - Days{1}).build();
    const Task taskRightRangeBorder =
        builder.withLastModificationStamp(rightmostDateTime).build();
    const Task taskOutOfRangeRight =
        builder.withLastModificationStamp(rightmostDateTime + Days{1}).build();
    const std::vector<Task> expected{taskLeftRangeBorder,
                                     insideRangeTask1,
                                     insideRangeTask2,
                                     taskRightRangeBorder};

    taskStorage->save(taskOutOfRangeLeft);
    taskStorage->save(taskLeftRangeBorder);
    taskStorage->save(insideRangeTask1);
    taskStorage->save(insideRangeTask2);
    taskStorage->save(taskRightRangeBorder);
    taskStorage->save(taskOutOfRangeRight);

    const auto tasks = taskStorage->allTasks(targetDateRange);

    ASSERT_EQ(expected.size(), tasks.size());
    ASSERT_EQ(expected, tasks);
}

// TEST_F(RequestingFinishedTasksFixture, requesting_finished_tasks)
// {
//     createTaskHandler.handle(
//         CreateTaskCommand{"Task name", {"Tag1", "Tag2"}, 4});
//     createTaskHandler.handle(
//         CreateTaskCommand{"Some other task", {"SomeTag"}, 2});
//     const auto uuids = activeTasksHandler.handle(ActiveTasksQuery{});
//     const auto uuid = uuids.front().uuid;
//     toggleTaskCompletedHandler.handle(
//         ToggleTaskCompletedCommand{uuid, current_date_time_local()});
//     TaskDTO expected{"",
//                      {"Tag1", "Tag2"},
//                      "Task name",
//                      4,
//                      0,
//                      true,
//                      dw::current_date_time_local()};
//
//     const auto finishedTasks = finishedTasksHandler.handle(FinishedTasksQuery{
//         DateRange{current_date_local(), current_date_local()}});
//
//     EXPECT_EQ(1, finishedTasks.size());
//     EXPECT_THAT(finishedTasks.front(),
//                 testing::Truly(matchers::MatchesTaskIgnoringUuid(expected)));
// }
