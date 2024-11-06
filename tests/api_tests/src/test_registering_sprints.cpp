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
#include "core/SprintConflictException.h"
#include "core/SprintTimerException.h"
#include "gtest/gtest.h"

using namespace sprint_timer;
using namespace sprint_timer::api;
using namespace sprint_timer;
using namespace sprint_timer::compose;
using namespace dw;
using namespace std::chrono_literals;

class RegisteringSprintsFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandComposer{
        initializer.commandHandlerComposer()};
    QueryHandlerComposer& queryComposer{initializer.queryHandlerComposer()};
    asp::QueryHandler<ActiveTasksQuery>& activeTasksHandler{
        queryComposer.activeTasksHandler()};
    asp::CommandHandler<RegisterSprintBulkCommand>& registerSprintsHandler{
        commandComposer.registerSprintBulkHandler()};
    asp::CommandHandler<CreateTaskCommand>& createTaskHandler{
        commandComposer.createTaskHandler()};
    asp::CommandHandler<UndoLastCommand>& undoCommandHandler{
        commandComposer.undoHandler()};
};

TEST_F(RegisteringSprintsFixture, registers_sprints)
{
    const DateTime taskCreatedStamp{
        DateTime{Date{Year{2024}, Month{12}, Day{27}}} + 17h + 37min};
    {
        TimePortalGuard timerPortal{initializer.getDateTimeProvider(),
                                    taskCreatedStamp};
        createTaskHandler.handle(
            CreateTaskCommand{"Some task",
                              {"Tag1"},
                              5,
                              TaskTypeDTO::Regular,
                              std::nullopt,
                              std::nullopt,
                              std::nullopt,
                              TaskTimeframeDTO{taskCreatedStamp}});
    }
    const DateTime currentTime{DateTime{Date{Year{2024}, Month{12}, Day{30}}} +
                               10h};
    const DateTime sprintRegistationTime = currentTime + 20h;
    const auto taskUuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    const DateTimeRange range{currentTime, currentTime + 25min};
    const std::vector<dw::DateTimeRange> intervals{range,
                                                   add_offset(range, 3h)};
    const TaskDTO expected{.uuid = taskUuid,
                           .tags = {"Tag1"},
                           .name = "Some task",
                           .expectedCost = 5,
                           .sprints = intervals,
                           .finished = false,
                           .modificationStamp = sprintRegistationTime,
                           .notes = std::nullopt,
                           .timeFrame = TaskTimeframeDTO{taskCreatedStamp},
                           .kind = TaskTypeDTO::Regular};

    TimePortalGuard timePortal{initializer.getDateTimeProvider(),
                               sprintRegistationTime};
    registerSprintsHandler.handle(
        RegisterSprintBulkCommand{taskUuid, intervals});

    const auto activeTasks = activeTasksHandler.handle(ActiveTasksQuery{});
    EXPECT_EQ(expected, activeTasks.front());
}

TEST_F(RegisteringSprintsFixture, undoing_registering_sprints)
{
    // TODO Startwith check that after undoing task modification stamp is
    // restored
    const DateTime taskCreatedStamp{
        DateTime{Date{Year{2024}, Month{12}, Day{27}}} + 17h + 37min};
    {
        TimePortalGuard timePortal{initializer.getDateTimeProvider(),
                                   taskCreatedStamp};
        createTaskHandler.handle(
            CreateTaskCommand{"Some task",
                              {"Tag1"},
                              5,
                              TaskTypeDTO::Regular,
                              std::nullopt,
                              std::nullopt,
                              std::nullopt,
                              TaskTimeframeDTO{taskCreatedStamp}});
    }
    const DateTime currentTime{DateTime{Date{Year{2024}, Month{12}, Day{30}}} +
                               10h};
    const auto taskUuid =
        activeTasksHandler.handle(ActiveTasksQuery{}).front().uuid;
    const DateTimeRange range{currentTime, currentTime + 25min};
    const std::vector<dw::DateTimeRange> intervals{range,
                                                   add_offset(range, 3h)};
    const TaskDTO expected{.uuid = taskUuid,
                           .tags = {"Tag1"},
                           .name = "Some task",
                           .expectedCost = 5,
                           .sprints = {},
                           .finished = false,
                           .modificationStamp = taskCreatedStamp,
                           .notes = std::nullopt,
                           .timeFrame = TaskTimeframeDTO{taskCreatedStamp},
                           .kind = TaskTypeDTO::Regular};
    {
        TimePortalGuard timePortal{initializer.getDateTimeProvider(),
                                   currentTime};
        registerSprintsHandler.handle(
            RegisterSprintBulkCommand{taskUuid, intervals});
    }

    undoCommandHandler.handle(UndoLastCommand{});

    const auto activeTasks = activeTasksHandler.handle(ActiveTasksQuery{});
    EXPECT_EQ(expected, activeTasks.front());
}

TEST_F(RegisteringSprintsFixture,
       throws_exception_when_sprint_intersection_detected)
{
    const DateTime currentTime{DateTime{Date{Year{2024}, Month{12}, Day{30}}} +
                               10h};
    const DateTimeRange range{currentTime, currentTime + 25min};
    const DateTime taskCreatedStamp{
        DateTime{Date{Year{2024}, Month{12}, Day{27}}} + 17h + 37min};
    {
        TimePortalGuard timePortal{initializer.getDateTimeProvider(),
                                   taskCreatedStamp};
        createTaskHandler.handle(
            CreateTaskCommand{.name = "Some task",
                              .tags = {"Tag1"},
                              .estimatedCost = 15,
                              .type = TaskTypeDTO::Regular,
                              .parent = std::nullopt,
                              .insertBeforePos = std::nullopt,
                              .notes = std::nullopt,
                              .timeFrame = TaskTimeframeDTO{taskCreatedStamp}});
        createTaskHandler.handle(
            CreateTaskCommand{"Another task",
                              {"SomeTag", "AnotherTag"},
                              7,
                              TaskTypeDTO::Regular,
                              std::nullopt,
                              std::nullopt,
                              std::nullopt,
                              TaskTimeframeDTO{taskCreatedStamp}});
    }
    const auto taskUuids = activeTasksHandler.handle(ActiveTasksQuery{});
    {
        TimePortalGuard timePortal{initializer.getDateTimeProvider(),
                                   currentTime};
        registerSprintsHandler.handle(RegisterSprintBulkCommand{
            taskUuids[0].uuid, {range, add_offset(range, 25min)}});
        registerSprintsHandler.handle(RegisterSprintBulkCommand{
            taskUuids[1].uuid,
            {add_offset(range, 2h), add_offset(range, 2h + 30min)}});
    }

    ASSERT_THROW(registerSprintsHandler.handle(RegisterSprintBulkCommand{
                     taskUuids[0].uuid, {add_offset(range, 2h - 24min)}}),
                 SprintConflictException);
}

TEST_F(
    RegisteringSprintsFixture,
    throws_exception_when_trying_to_add_sprints_that_are_conflicting_between_themselves)
{
    const DateTime currentTime{DateTime{Date{Year{2024}, Month{12}, Day{30}}} +
                               10h};
    const DateTimeRange range{currentTime, currentTime + 25min};
    const DateTime taskCreatedStamp{
        DateTime{Date{Year{2024}, Month{12}, Day{27}}} + 17h + 37min};
    {
        TimePortalGuard timePortal{initializer.getDateTimeProvider(),
                                   taskCreatedStamp};
        createTaskHandler.handle(
            CreateTaskCommand{"Some task",
                              {"Tag1"},
                              15,
                              TaskTypeDTO::Regular,
                              std::nullopt,
                              std::nullopt,
                              std::nullopt,
                              TaskTimeframeDTO{taskCreatedStamp}});
    }
    const auto taskUuids = activeTasksHandler.handle(ActiveTasksQuery{});

    ASSERT_THROW(registerSprintsHandler.handle(RegisterSprintBulkCommand{
                     taskUuids[0].uuid, {range, add_offset(range, 12min)}}),
                 SprintConflictException);
}

TEST_F(RegisteringSprintsFixture,
       throws_when_registering_sprint_for_task_that_is_not_found)
{
    const DateTime currentTime{DateTime{Date{Year{2024}, Month{12}, Day{30}}} +
                               10h};
    const std::string bogusUuid{"123"};
    const DateTimeRange range{currentTime, currentTime + 25min};

    ASSERT_THROW(registerSprintsHandler.handle(
                     RegisterSprintBulkCommand{bogusUuid, {range}}),
                 SprintTimerException);
}
