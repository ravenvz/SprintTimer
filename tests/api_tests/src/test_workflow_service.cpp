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

#include "api/WorkflowService.h"
#include "api/requests/RegisterSprintBulkCommand.h"
#include "api_tests/QtStorageInitializer.h"
#include "common_utils/DateTimeProviderMock.h"
#include "core/SprintTimer.h"
// #include "api/requests/StartTimerCommand.h"

using namespace sprint_timer;
using namespace sprint_timer::compose;
using namespace sprint_timer::api;
using namespace dw;
using namespace std::chrono_literals;
using asp::CommandHandler;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::ResultOf;
using ::testing::Return;

namespace {

constexpr std::chrono::seconds sprintDuration{1500};
constexpr std::chrono::seconds shortBreakDuration{300};
constexpr std::chrono::seconds longBreakDuration{900};
constexpr std::chrono::milliseconds tickPeriod{1000};
constexpr int32_t sprintsBeforeBreakStarted{4};
constexpr SprintTimer::WorkflowParams workflowParams{sprintDuration,
                                                     shortBreakDuration,
                                                     longBreakDuration,
                                                     sprintsBeforeBreakStarted};
constexpr std::chrono::minutes sprintDurationInMinutes =
    std::chrono::duration_cast<std::chrono::minutes>(
        workflowParams.sprintDuration);

} // namespace

class TimerWorkflowListenerMock : public TimerWorkflowListener {
public:
    MOCK_METHOD(void, onIdle, (), (override));

    MOCK_METHOD(void, onSprintStarted, (std::chrono::seconds), (override));

    MOCK_METHOD(void,
                onSprintFinished,
                (std::vector<dw::DateTimeRange>&&),
                (override));

    MOCK_METHOD(void, onBreakStarted, (std::chrono::seconds), (override));

    MOCK_METHOD(void, onBreakFinished, (), (override));

    MOCK_METHOD(void, onZoneEntered, (), (override));

    MOCK_METHOD(void, onZoneLeft, (), (override));

    MOCK_METHOD(void, onTimerTick, (std::chrono::seconds), (override));
};

class CountdownTimerMock : public CountdownTimer {
public:
    MOCK_METHOD(void,
                runImpl,
                (TimerDuration, TickPeriod, OnTick, OnRunout),
                (override));

    MOCK_METHOD(void, stopImpl, (), (override));

    MOCK_METHOD(void, setLoopImpl, (bool), (override));
};

auto convertTickPeriod = [](const CountdownTimer::TickPeriod& tp) {
    return static_cast<std::chrono::milliseconds>(tp);
};
auto convertDuration = [](const CountdownTimer::TimerDuration& td) {
    return static_cast<std::chrono::milliseconds>(td);
};

class WorkflowServiceFixture : public ::testing::Test {
public:
    TestStorageInitializer initializer;
    CommandHandlerComposer& commandHandlerComposer{
        initializer.commandHandlerComposer()};
    CommandHandler<RegisterSprintBulkCommand>& registerSprintsHandler{
        commandHandlerComposer.registerSprintBulkHandler()};
    NiceMock<TimerWorkflowListenerMock> listener;
    NiceMock<mocks::DateTimeProviderMock> dateTimeProvider;
    CommandHandler<StartTimerCommand>& startTimerHandler =
        commandHandlerComposer.startTimerHandler();
    CommandHandler<CancelWorkflowCommand>& cancelWorkflowHandler =
        commandHandlerComposer.cancelWorkflowHandler();
    CommandHandler<ToggleZoneCommand>& toggleZoneHandler =
        commandHandlerComposer.toggleZoneHandler();
    DateTime someTime =
        DateTime{Date{Year{2024}, Month{11}, Day{21}}} + 22h + 7min;

    auto SetUp() -> void override
    {
        initializer.installWorkflowListener(&listener);
    }

    auto expect_no_calls_to_listener()
    {
        EXPECT_CALL(listener, onTimerTick(_)).Times(0);
        EXPECT_CALL(listener, onIdle()).Times(0);
        EXPECT_CALL(listener, onSprintStarted(_)).Times(0);
        EXPECT_CALL(listener, onSprintFinished(_)).Times(0);
        EXPECT_CALL(listener, onBreakStarted(_)).Times(0);
        EXPECT_CALL(listener, onBreakFinished()).Times(0);
        EXPECT_CALL(listener, onZoneEntered()).Times(0);
        EXPECT_CALL(listener, onZoneLeft()).Times(0);
    }

    auto expect_only_tick_calls_to_listener()
    {
        EXPECT_CALL(listener, onIdle()).Times(0);
        EXPECT_CALL(listener, onSprintStarted(_)).Times(0);
        EXPECT_CALL(listener, onSprintFinished(_)).Times(0);
        EXPECT_CALL(listener, onBreakStarted(_)).Times(0);
        EXPECT_CALL(listener, onBreakFinished()).Times(0);
        EXPECT_CALL(listener, onZoneEntered()).Times(0);
        EXPECT_CALL(listener, onZoneLeft()).Times(0);
    }
};

TEST_F(WorkflowServiceFixture, starting_sprint_from_idle)
{
    // FAIL();
    ON_CALL(dateTimeProvider, dateTimeLocalNowImpl)
        .WillByDefault(Return(someTime));

    EXPECT_CALL(listener, onSprintStarted(sprintDuration));

    startTimerHandler.handle(StartTimerCommand{});
}

// TEST_F(WorkflowServiceFixture,
//        should_transition_from_sprint_to_finished_on_timer_runout)
// {
//     ON_CALL(dateTimeProvider, dateTimeLocalNowImpl)
//         .WillByDefault(Return(someTime));
//     sut.handle(StartTimerCommand{});
//
//     EXPECT_CALL(listener, onSprintFinished());
//
//     workflow.process(SprintTimer::TimerFinishedEvent{});
// }
//
// TEST_F(WorkflowServiceFixture, should_transition_from_finished_to_break)
// {
//     ON_CALL(dateTimeProvider, dateTimeLocalNowImpl)
//         .WillByDefault(Return(someTime));
//     sut.handle(StartTimerCommand{});
//     workflow.process(SprintTimer::TimerFinishedEvent{});
//
//     EXPECT_CALL(listener, onBreakStarted(shortBreakDuration));
//     EXPECT_CALL(countdownTimer,
//                 runImpl(ResultOf(convertDuration, shortBreakDuration),
//                         ResultOf(convertTickPeriod, tickPeriod),
//                         _,
//                         _));
//
//     sut.handle(SubmitCommand{"123"});
// }
//
// TEST_F(WorkflowServiceFixture,
//        should_transition_from_finished_to_long_break_when_condition_met)
// {
//     ON_CALL(dateTimeProvider, dateTimeLocalNowImpl)
//         .WillByDefault(Return(someTime));
//     workflow.setNumFinishedSprints(3);
//     sut.handle(StartTimerCommand{});
//     workflow.process(SprintTimer::TimerFinishedEvent{});
//
//     EXPECT_CALL(listener, onBreakStarted(longBreakDuration));
//     EXPECT_CALL(countdownTimer,
//                 runImpl(ResultOf(convertDuration, longBreakDuration),
//                         ResultOf(convertTickPeriod, tickPeriod),
//                         _,
//                         _));
//
//     sut.handle(SubmitCommand{"123"});
// }
//
// TEST_F(WorkflowServiceFixture, should_transition_from_break_to_idle)
// {
//     ON_CALL(dateTimeProvider, dateTimeLocalNowImpl)
//         .WillByDefault(Return(someTime));
//     sut.handle(StartTimerCommand{});
//     workflow.process(SprintTimer::TimerFinishedEvent{});
//     sut.handle(SubmitCommand{"123"});
//
//     EXPECT_CALL(listener, onIdle());
//
//     workflow.process(SprintTimer::TimerFinishedEvent{});
// }
//
// TEST_F(WorkflowServiceFixture,
//        should_transition_to_from_sprint_to_zone_when_toggled)
// {
//     ON_CALL(dateTimeProvider, dateTimeLocalNowImpl)
//         .WillByDefault(Return(someTime));
//     sut.handle(StartTimerCommand{});
//
//     EXPECT_CALL(countdownTimer, setLoopImpl(true));
//     EXPECT_CALL(listener, onZoneEntered());
//
//     sut.handle(ToggleZoneCommand{});
// }
//
// TEST_F(WorkflowServiceFixture,
//        should_transition_from_zone_to_sprint_when_toggled)
// {
//     ON_CALL(dateTimeProvider, dateTimeLocalNowImpl)
//         .WillByDefault(Return(someTime));
//     sut.handle(StartTimerCommand{});
//     sut.handle(ToggleZoneCommand{});
//
//     EXPECT_CALL(countdownTimer, setLoopImpl(false));
//     EXPECT_CALL(listener, onZoneLeft());
//
//     sut.handle(ToggleZoneCommand{});
// }
//
// TEST_F(WorkflowServiceFixture, idle_state_cannot_be_cancelled)
// {
//     expect_no_calls_to_listener();
//     expect_no_calls_to_timer();
//
//     sut.handle(CancelWorkflowCommand{});
// }
//
// TEST_F(WorkflowServiceFixture, zone_state_cannot_be_cancelled)
// {
//     ON_CALL(dateTimeProvider, dateTimeLocalNowImpl)
//         .WillByDefault(Return(someTime));
//     sut.handle(StartTimerCommand{});
//     sut.handle(ToggleZoneCommand{});
//
//     expect_no_calls_to_listener();
//     expect_no_calls_to_timer();
//
//     sut.handle(CancelWorkflowCommand{});
// }
//
// TEST_F(WorkflowServiceFixture,
// sprint_states_transitions_to_idle_when_cancelled)
// {
//     ON_CALL(dateTimeProvider, dateTimeLocalNowImpl)
//         .WillByDefault(Return(someTime));
//     sut.handle(StartTimerCommand{});
//
//     EXPECT_CALL(listener, onIdle());
//     EXPECT_CALL(countdownTimer, stopImpl());
//
//     sut.handle(CancelWorkflowCommand{});
// }
//
// TEST_F(WorkflowServiceFixture,
// break_state_transitions_to_idle_when_cancelled)
// {
//     ON_CALL(dateTimeProvider, dateTimeLocalNowImpl)
//         .WillByDefault(Return(someTime));
//     sut.handle(StartTimerCommand{});
//     workflow.process(SprintTimer::TimerFinishedEvent{});
//     sut.handle(SubmitCommand{"123"});
//
//     EXPECT_CALL(listener, onIdle());
//     EXPECT_CALL(countdownTimer, stopImpl());
//
//     sut.handle(CancelWorkflowCommand{});
// }
//
// TEST_F(WorkflowServiceFixture,
//        sprint_finished_state_should_transition_to_idle_when_cancelled)
// {
//     ON_CALL(dateTimeProvider, dateTimeLocalNowImpl)
//         .WillByDefault(Return(someTime));
//     sut.handle(StartTimerCommand{});
//     workflow.process(SprintTimer::TimerFinishedEvent{});
//
//     EXPECT_CALL(listener, onIdle());
//
//     sut.handle(CancelWorkflowCommand{});
// }
//
// TEST_F(WorkflowServiceFixture,
// zone_transition_should_be_ignored_in_idle_state)
// {
//     expect_only_tick_calls_to_listener();
//     expect_no_calls_to_timer();
//
//     sut.handle(ToggleZoneCommand{});
// }
//
// TEST_F(WorkflowServiceFixture,
// zone_transition_should_be_ignored_in_break_state)
// {
//     ON_CALL(dateTimeProvider, dateTimeLocalNowImpl)
//         .WillByDefault(Return(someTime));
//     sut.handle(StartTimerCommand{});
//     workflow.process(SprintTimer::TimerFinishedEvent{});
//     sut.handle(SubmitCommand{"123"});
//
//     expect_only_tick_calls_to_listener();
//     expect_no_calls_to_timer();
//
//     sut.handle(ToggleZoneCommand{});
// }
//
// TEST_F(WorkflowServiceFixture,
//        zone_transition_should_be_ignored_in_finished_state)
// {
//     ON_CALL(dateTimeProvider, dateTimeLocalNowImpl)
//         .WillByDefault(Return(someTime));
//     sut.handle(StartTimerCommand{});
//     workflow.process(SprintTimer::TimerFinishedEvent{});
//
//     expect_only_tick_calls_to_listener();
//     expect_no_calls_to_timer();
//
//     sut.handle(ToggleZoneCommand{});
// }
//
// TEST_F(WorkflowServiceFixture, zone_mode_accumulates_sprints)
// {
//     // TODO wire action invoker and ensure it is called
// }
