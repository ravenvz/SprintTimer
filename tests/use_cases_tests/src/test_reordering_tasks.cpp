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
#include "use_cases_tests/utils/utils.h"
#include "gmock/gmock.h"

using namespace sprint_timer;
using namespace sprint_timer::use_cases;
using namespace sprint_timer::entities;
using namespace sprint_timer::compose;
using namespace dw;

using ::testing::ElementsAre;

class ReorderingTasksFixture : public ::testing::Test {
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
    CommandHandler<ChangeActiveTasksPriorityCommand>& reorderTasksHandler{
        commandComposer.reorderTasksHandler()};
};

TEST_F(ReorderingTasksFixture, changing_active_tasks_order)
{
    createTaskHandler.handle(CreateTaskCommand{"Task 1", {"Tag1"}, 1});
    createTaskHandler.handle(CreateTaskCommand{"Task 2", {"Tag2"}, 2});
    createTaskHandler.handle(CreateTaskCommand{"Task 3", {"Tag3"}, 3});
    createTaskHandler.handle(CreateTaskCommand{"Task 4", {"Tag4"}, 4});
    const auto tasks = activeTasksHandler.handle(ActiveTasksQuery{});
    const auto uuids =
        extractUuids(activeTasksHandler.handle(ActiveTasksQuery{}));
    std::vector<std::string> reordered{uuids[3], uuids[2], uuids[0], uuids[1]};

    reorderTasksHandler.handle(
        ChangeActiveTasksPriorityCommand{uuids, reordered});

    EXPECT_THAT(extractUuids(activeTasksHandler.handle(ActiveTasksQuery{})),
                ElementsAre(uuids[3], uuids[2], uuids[0], uuids[1]));
}
