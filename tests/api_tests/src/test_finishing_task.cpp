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
    DateTime anchorTime{DateTime{Date{Year{2021}, Month{5}, Day{4}}}};
    TestStorageInitializer initializer{anchorTime};
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
    asp::QueryHandler<ReadTaskTreeQuery>& readTaskTreeHandler{
        queryComposer.readTaskTreeHandler()};
};

TEST_F(FinishingTaskFixture, toggling_task_completion_alters_timestamp)
{
    const DateTime timeStamp = current_date_time_local();
    createTaskHandler.handle(CreateTaskCommand{"Name",
                                               {"Tag9"},
                                               7,
                                               TaskTypeDTO::Regular,
                                               std::nullopt,
                                               std::nullopt,
                                               std::nullopt,
                                               TaskTimeframeDTO{}});
    const std::string next_generated_uuid = "0";
    const TaskDTO expected{next_generated_uuid,
                           {"Tag9"},
                           "Name",
                           7,
                           {},
                           true,
                           timeStamp,
                           std::nullopt,
                           TaskTimeframeDTO{},
                           TaskTypeDTO::Regular};

    // NOTE date time provider timepoint is being altered here
    TimePortalGuard timePortal{initializer.getDateTimeProvider(), timeStamp};
    toggleTaskCompletedHandler.handle(
        ToggleTaskCompletedCommand{next_generated_uuid});

    EXPECT_EQ(expected, activeTasksHandler.handle(ActiveTasksQuery{}).front());
}

TEST_F(
    FinishingTaskFixture,
    undoing_task_completion_for_non_recurring_task_preserves_modification_stamp)
{
    const dw::DateTime hundredDaysAgo = anchorTime - dw::Days{100};
    {
        TimePortalGuard timePortal{initializer.getDateTimeProvider(),
                                   hundredDaysAgo};
        createTaskHandler.handle(CreateTaskCommand{"Name",
                                                   {"Tag9"},
                                                   7,
                                                   TaskTypeDTO::Regular,
                                                   std::nullopt,
                                                   std::nullopt,
                                                   std::nullopt,
                                                   TaskTimeframeDTO{}});
    }
    const auto task = activeTasksHandler.handle(ActiveTasksQuery{}).front();
    const TaskDTO expected{task.uuid,
                           {"Tag9"},
                           "Name",
                           7,
                           {},
                           false,
                           hundredDaysAgo,
                           std::nullopt,
                           TaskTimeframeDTO{},
                           TaskTypeDTO::Regular};
    toggleTaskCompletedHandler.handle(ToggleTaskCompletedCommand{task.uuid});

    undoHandler.handle(UndoLastCommand{});

    EXPECT_EQ(expected, activeTasksHandler.handle(ActiveTasksQuery{}).front());
}

TEST_F(FinishingTaskFixture,
       finishing_recurring_task_creates_another_task_instance)
{
    const DateTime firstRecurrence{Date{Year{2023}, Month{6}, Day{5}}};
    const DateTime secondRecurrence{Date{Year{2023}, Month{6}, Day{12}}};
    const DateTime initialStart = DateTime{Date{Year{2023}, Month{2}, Day{5}}};
    const DateTime currentTime =
        DateTime{Date{Year{2023}, Month{4}, Day{17}}} + 12h + 11min;
    const std::string normalizedRecurrenceStr = "Mon *-06..12-* 00:00:00";
    createTaskHandler.handle(
        CreateTaskCommand{"Name",
                          {"Tag1"},
                          7,
                          TaskTypeDTO::Regular,
                          std::nullopt,
                          std::nullopt,
                          std::nullopt,
                          TaskTimeframeDTO{initialStart,
                                           std::nullopt,
                                           std::nullopt,
                                           normalizedRecurrenceStr}});
    std::vector<dw::DateTimeRange> sprints{DateTimeRange{
        DateTime{Date{Year{2023}, Month{5}, Day{4}}} + 1h + 25min,
        DateTime{Date{Year{2023}, Month{5}, Day{4}}} + 1h + 50min}};
    TaskTreeDTO expected;
    expected.insert(expected.end(),
                    TaskDTO{"1",
                            {"Tag1"},
                            "Name",
                            7,
                            {},
                            false,
                            currentTime,
                            std::nullopt,
                            TaskTimeframeDTO{firstRecurrence,
                                             secondRecurrence,
                                             std::nullopt,
                                             normalizedRecurrenceStr},
                            TaskTypeDTO::Regular});
    expected.insert(expected.end(),
                    TaskDTO{"0",
                            {"Tag1"},
                            "Name",
                            7,
                            sprints,
                            true,
                            currentTime,
                            std::nullopt,
                            TaskTimeframeDTO{initialStart,
                                             firstRecurrence,
                                             std::nullopt,
                                             normalizedRecurrenceStr},
                            TaskTypeDTO::Regular});

    TimePortalGuard timePortal{initializer.getDateTimeProvider(), currentTime};
    registerSprintsHandler.handle(RegisterSprintBulkCommand{"0", sprints});
    toggleTaskCompletedHandler.handle(ToggleTaskCompletedCommand{"0"});
    const auto tree = readTaskTreeHandler.handle(ReadTaskTreeQuery{});

    EXPECT_EQ(expected, tree);
}

TEST_F(FinishingTaskFixture,
       undoing_finishing_recurring_task_removes_created_task_instance)
{
    const DateTime firstRecurrence{Date{Year{2023}, Month{6}, Day{5}}};
    const DateTime secondRecurrence{Date{Year{2023}, Month{6}, Day{12}}};
    const DateTime initialStart = DateTime{Date{Year{2023}, Month{2}, Day{5}}};
    const DateTime modificationTime =
        DateTime{Date{Year{2023}, Month{4}, Day{17}}} + 12h + 11min;
    const DateTime taskCompletionTime =
        DateTime{Date{Year{2023}, Month{4}, Day{18}}} + 1h;
    std::vector<dw::DateTimeRange> sprints{DateTimeRange{
        DateTime{Date{Year{2023}, Month{5}, Day{4}}} + 1h + 25min,
        DateTime{Date{Year{2023}, Month{5}, Day{4}}} + 1h + 50min}};
    const std::string normalizedRecurrenceStr = "Mon *-06..12-* 00:00:00";
    {
        TimePortalGuard timePortal{initializer.getDateTimeProvider(),
                                   modificationTime};
        createTaskHandler.handle(
            CreateTaskCommand{"Name",
                              {"Tag1"},
                              7,
                              TaskTypeDTO::Regular,
                              std::nullopt,
                              std::nullopt,
                              std::nullopt,
                              TaskTimeframeDTO{initialStart,
                                               std::nullopt,
                                               std::nullopt,
                                               normalizedRecurrenceStr}});
        registerSprintsHandler.handle(RegisterSprintBulkCommand{"0", sprints});
    }
    TaskTreeDTO expected;
    expected.insert(expected.end(),
                    TaskDTO{"0",
                            {"Tag1"},
                            "Name",
                            7,
                            sprints,
                            false,
                            modificationTime,
                            std::nullopt,
                            TaskTimeframeDTO{initialStart,
                                             firstRecurrence,
                                             std::nullopt,
                                             normalizedRecurrenceStr},
                            TaskTypeDTO::Regular});
    {
        TimePortalGuard timePortal{initializer.getDateTimeProvider(),
                                   taskCompletionTime};
        toggleTaskCompletedHandler.handle(ToggleTaskCompletedCommand{"0"});
        undoHandler.handle(UndoLastCommand{});
    }

    const auto tree = readTaskTreeHandler.handle(ReadTaskTreeQuery{});

    EXPECT_EQ(expected, tree);
}

TEST_F(FinishingTaskFixture,
       finishing_recurring_task_regenerates_finished_nonrecurring_subtasks)
{
    const DateTime startTime{DateTime{Date{Year{2024}, Month{10}, Day{3}}} +
                             11h + 12min};
    const DateTime modificationTime{
        DateTime{Date{Year{2024}, Month{15}, Day{4}}} + 11h + 12min};
    const DateTime completionTime{
        DateTime{Date{Year{2024}, Month{15}, Day{5}}} + 1h};
    {
        TimePortalGuard timePortal{initializer.getDateTimeProvider(),
                                   startTime};
        // uuid = 0
        createTaskHandler.handle(CreateTaskCommand{
            "Recurring parent task",
            {},
            1,
            TaskTypeDTO::Regular,
            std::nullopt,
            std::nullopt,
            std::nullopt,
            TaskTimeframeDTO{
                startTime, std::nullopt, std::nullopt, "*-*-* 00:04:00"}});
        // uuid = 1
        createTaskHandler.handle(CreateTaskCommand{
            "Non recurring subtask",
            {},
            4,
            TaskTypeDTO::Regular,
            std::nullopt,
            std::nullopt,
            std::nullopt,
            TaskTimeframeDTO{
                startTime, std::nullopt, std::nullopt, std::nullopt}});
        // uuid = 2
        createTaskHandler.handle(CreateTaskCommand{
            "Recurring subtask",
            {},
            1,
            TaskTypeDTO::Regular,
            std::nullopt,
            std::nullopt,
            std::nullopt,
            TaskTimeframeDTO{
                startTime, std::nullopt, std::nullopt, "*-*-12 00:07:00"}});
    }
    {

        TimePortalGuard timePortal{initializer.getDateTimeProvider(),
                                   modificationTime};
        registerSprintsHandler.handle(RegisterSprintBulkCommand{
            "1",
            {DateTimeRange{modificationTime - 25min, modificationTime - 50min},
             DateTimeRange{modificationTime, modificationTime - 25min}}});
        toggleTaskCompletedHandler.handle(ToggleTaskCompletedCommand{"1"});
    }
    TaskTreeDTO expected;
    auto parent = expected.insert(
        expected.end(),
        TaskDTO{
            "0",
            {},
            "Recurring parent task",
            1,
            {},
            false,
            completionTime,
            std::nullopt,
            TaskTimeframeDTO{DateTime{Date{Year{2024}, Month{10}, Day{4}}} + 4h,
                             DateTime{Date{Year{2024}, Month{10}, Day{5}}} + 4h,
                             std::nullopt,
                             "*-*-* 00:04:00"},
            TaskTypeDTO::Regular});
    expected.insert(
        parent,
        TaskDTO{"1",
                {},
                "Non recurring subtask",
                4,
                {},
                false,
                completionTime,
                std::nullopt,
                TaskTimeframeDTO{
                    completionTime, std::nullopt, std::nullopt, std::nullopt},
                TaskTypeDTO::Regular});
    expected.insert(
        parent,
        TaskDTO{"2",
                {},
                "Recurrent subtask",
                1,
                {},
                false,
                startTime,
                std::nullopt,
                TaskTimeframeDTO{
                    startTime, std::nullopt, std::nullopt, "*-*-12 00:07:00"},
                TaskTypeDTO::Regular});

    toggleTaskCompletedHandler.handle(ToggleTaskCompletedCommand{"0"});

    EXPECT_EQ(expected, readTaskTreeHandler.handle(ReadTaskTreeQuery{}));
}

