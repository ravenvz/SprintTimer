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
#include "gmock/gmock.h"

#include "use_cases_tests/QtStorageInitializer.h"
#include "use_cases_tests/matchers/MatchesTaskIgnoringUuid.h"
#include "gmock/gmock.h"

using namespace sprint_timer;
using namespace sprint_timer::use_cases;
using namespace sprint_timer::entities;
using namespace sprint_timer::compose;
using namespace dw;

using ::testing::ElementsAre;

class RequestingActiveTasksFixture : public ::testing::Test {
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
    CommandHandler<ToggleTaskCompletedCommand>& toggleTaskCompletedHandler{
        commandComposer.toggleTaskCompletedHandler()};
};

TEST_F(
    RequestingActiveTasksFixture,
    requesting_active_tasks_treating_recently_modified_finished_task_as_active)
{
    createTaskHandler.handle(
        CreateTaskCommand{"Task name", {"Tag1", "Tag2"}, 4});
    createTaskHandler.handle(
        CreateTaskCommand{"Some other task", {"SomeTag"}, 2});
    const auto tasks = activeTasksHandler.handle(ActiveTasksQuery{});
    std::vector<std::string> uuids;
    std::transform(cbegin(tasks),
                   cend(tasks),
                   std::back_inserter(uuids),
                   [](const auto& elem) { return elem.uuid; });
    toggleTaskCompletedHandler.handle(
        ToggleTaskCompletedCommand{uuids.front(), current_date_time_local()});

    EXPECT_THAT(activeTasksHandler.handle(ActiveTasksQuery{}),
                ElementsAre(TaskDTO{uuids.front(),
                                    {"Tag1", "Tag2"},
                                    "Task name",
                                    4,
                                    0,
                                    true,
                                    current_date_time_local()},
                            TaskDTO{uuids.back(),
                                    {"SomeTag"},
                                    "Some other task",
                                    2,
                                    0,
                                    false,
                                    current_date_time_local()}));
}
