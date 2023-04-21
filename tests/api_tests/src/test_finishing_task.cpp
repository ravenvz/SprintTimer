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
#include "gtest/gtest.h"

using namespace sprint_timer;
using namespace sprint_timer::api;
using namespace sprint_timer;
using namespace sprint_timer::compose;
using namespace dw;
using namespace std::chrono_literals;

class FinishingTaskFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    asp::CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandComposer.createTaskHandler()};
    asp::QueryHandler<ActiveTasksQuery>& activeTasksHandler{
        queryComposer.activeTasksHandler()};
    asp::CommandHandler<ToggleTaskCompletedCommand>& toggleTaskCompletedHandler{
        commandComposer.toggleTaskCompletedHandler()};
    asp::CommandHandler<UndoLastCommand>& undoHandler{
        commandComposer.undoHandler()};
    asp::CommandHandler<RegisterSprintBulkCommand>& registerSprintsHandler{
        commandComposer.registerSprintBulkHandler()};
};

TEST_F(FinishingTaskFixture, toggling_task_completion_alters_timestamp)
{
    const DateTime timeStamp{DateTime{Date{Year{2021}, Month{5}, Day{4}}}};
    createTaskHandler.handle(CreateTaskCommand{"Name",
                                               {"Tag9"},
                                               7,
                                               TaskTypeDTO::Regular,
                                               std::nullopt,
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{}});
    const auto uuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    const TaskDTO expected{uuid,
                           {"Tag9"},
                           "Name",
                           7,
                           {},
                           true,
                           current_date_time_local(),
                           std::nullopt,
                           TaskTimeframeDTO{},
                           TaskTypeDTO::Regular};

    toggleTaskCompletedHandler.handle(
        ToggleTaskCompletedCommand{uuid, timeStamp});

    EXPECT_EQ(expected, activeTasksHandler.handle(ActiveTasksQuery{}).front());
}

TEST_F(FinishingTaskFixture, undoing_task_completion_for_non_recurring_task)
{
    const DateTime timeStamp{DateTime{Date{Year{2021}, Month{5}, Day{4}}}};
    createTaskHandler.handle(CreateTaskCommand{"Name",
                                               {"Tag9"},
                                               7,
                                               TaskTypeDTO::Regular,
                                               std::nullopt,
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{}});
    const auto uuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    const TaskDTO expected{uuid,
                           {"Tag9"},
                           "Name",
                           7,
                           {},
                           false,
                           timeStamp,
                           std::nullopt,
                           TaskTimeframeDTO{},
                           TaskTypeDTO::Regular};
    toggleTaskCompletedHandler.handle(
        ToggleTaskCompletedCommand{uuid, timeStamp});

    undoHandler.handle(UndoLastCommand{});

    EXPECT_EQ(expected, activeTasksHandler.handle(ActiveTasksQuery{}).front());
}

TEST_F(FinishingTaskFixture,
       finishing_recurring_task_creates_another_task_instance)
{
    const DateTime timeStamp{DateTime{Date{Year{2023}, Month{5}, Day{8}}}};
    createTaskHandler.handle(CreateTaskCommand{
        "Name",
        {"Tag1"},
        7,
        TaskTypeDTO::Regular,
        std::nullopt,
        std::nullopt,
        std::nullopt,
        TaskTimeframeDTO{DateTime{Date{Year{2023}, Month{2}, Day{5}}},
                         std::nullopt,
                         std::nullopt,
                         "Mon *-6..12-*"}});
    std::vector<dw::DateTimeRange> sprints{DateTimeRange{
        DateTime{Date{Year{2023}, Month{5}, Day{4}}} + 1h + 25min,
        DateTime{Date{Year{2023}, Month{5}, Day{4}}} + 1h + 50min}};
    registerSprintsHandler.handle(RegisterSprintBulkCommand{"0", sprints});
    std::vector<TaskDTO> expected{
        TaskDTO{"0",
                {"Tag1"},
                "Name",
                7,
                sprints,
                true,
                dw::current_date_time_local(),
                std::nullopt,
                TaskTimeframeDTO{DateTime{Date{Year{2023}, Month{2}, Day{5}}},
                                 std::nullopt,
                                 std::nullopt,
                                 "Mon *-6..12-*"},
                TaskTypeDTO::Regular},
        TaskDTO{"1",
                {"Tag1"},
                "Name",
                7,
                {},
                false,
                dw::current_date_time_local(),
                std::nullopt,
                TaskTimeframeDTO{DateTime{Date{Year{2023}, Month{2}, Day{5}}},
                                 std::nullopt,
                                 std::nullopt,
                                 "Mon *-6..12-*"},
                TaskTypeDTO::Regular}};

    toggleTaskCompletedHandler.handle(
        ToggleTaskCompletedCommand{"0", dw::current_date_time_local()});
    const auto tasks = activeTasksHandler.handle(ActiveTasksQuery{});

    EXPECT_EQ(expected, tasks);
}

TEST_F(FinishingTaskFixture,
       undoing_finishing_recurring_task_removes_created_task_instance)
{
    FAIL();
}

TEST_F(
    FinishingTaskFixture,
    finishing_recurring_task_regenerates_all_completed_subtasks_and_updates_due_time_for_all_tasks)
{
    FAIL();
}
