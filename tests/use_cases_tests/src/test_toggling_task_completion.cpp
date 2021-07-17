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

class TogglingTaskCompletionFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandComposer.createTaskHandler()};
    QueryHandler<ActiveTasksQuery>& activeTasksHandler{
        queryComposer.activeTasksHandler()};
    CommandHandler<ToggleTaskCompletedCommand>& toggleTaskCompletedHandler{
        commandComposer.toggleTaskCompletedHandler()};
};

TEST_F(TogglingTaskCompletionFixture, toggling_task_completion_alters_timestamp)
{
    const DateTime timeStamp{DateTime{Date{Year{2021}, Month{5}, Day{4}}}};
    createTaskHandler.handle(CreateTaskCommand{"Name", {"Tag9"}, 7});
    const auto uuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    const TaskDTO expected{
        uuid, {"Tag9"}, "Name", 7, 0, true, current_date_time_local()};

    toggleTaskCompletedHandler.handle(
        ToggleTaskCompletedCommand{uuid, timeStamp});

    EXPECT_EQ(expected, activeTasksHandler.handle(ActiveTasksQuery{}).front());
}
