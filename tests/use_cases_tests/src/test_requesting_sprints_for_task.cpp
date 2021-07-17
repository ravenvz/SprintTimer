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
#include "use_cases_tests/matchers/MatchesSprintIgnoringUuid.h"
#include "gmock/gmock.h"

using namespace sprint_timer;
using namespace sprint_timer::use_cases;
using namespace sprint_timer::entities;
using namespace sprint_timer::compose;
using namespace dw;

using ::testing::Truly;

class SprintsForTaskFixture : public ::testing::Test {
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
    CommandHandler<RegisterSprintBulkCommand>& registerSprintHandler{
        commandComposer.registerSprintBulkHandler()};
    QueryHandler<SprintsForTaskQuery>& sprintsForTaskHandler{
        queryComposer.sprintsForTaskHandler()};
};

TEST_F(SprintsForTaskFixture, all_sprints_for_specific_task)
{
    using namespace std::chrono_literals;
    using matchers::MatchesSprintIgnoringUuid;
    createTaskHandler.handle(
        CreateTaskCommand{"First task", {"Tag1", "Tag2"}, 8});
    createTaskHandler.handle(
        CreateTaskCommand{"Another task", {"Tag2", "Tag3"}, 4});
    const auto activeTasks = activeTasksHandler.handle(ActiveTasksQuery{});
    std::vector<std::string> uuids(2, "");
    std::transform(cbegin(activeTasks),
                   cend(activeTasks),
                   begin(uuids),
                   [](const auto& elem) { return elem.uuid; });
    const DateTime someTime{current_date_time_local()};
    const DateTimeRange someRange{someTime, someTime + 25min};
    registerSprintHandler.handle(RegisterSprintBulkCommand{
        uuids.front(), {someRange, add_offset(someRange, 1h)}});
    registerSprintHandler.handle(
        RegisterSprintBulkCommand{uuids.back(),
                                  {add_offset(someRange, 2h),
                                   add_offset(someRange, 3h),
                                   add_offset(someRange, 4h)}});

    const auto actual =
        sprintsForTaskHandler.handle(SprintsForTaskQuery{uuids.back()});

    EXPECT_THAT(
        actual,
        ::testing::ElementsAre(Truly(MatchesSprintIgnoringUuid(
                                   SprintDTO{"any",
                                             uuids.back(),
                                             "Another task",
                                             {"Tag2", "Tag3"},
                                             add_offset(someRange, 2h)})),
                               Truly(MatchesSprintIgnoringUuid(
                                   SprintDTO{"anything",
                                             uuids.back(),
                                             "Another task",
                                             {"Tag2", "Tag3"},
                                             add_offset(someRange, 3h)})),
                               Truly(MatchesSprintIgnoringUuid(
                                   SprintDTO{"whatever",
                                             uuids.back(),
                                             "Another task",
                                             {"Tag2", "Tag3"},
                                             add_offset(someRange, 4h)}))));
}
