#include "core/SprintTimer.h"
#include "gmock/gmock.h"

using namespace sprint_timer;
using namespace dw;
using namespace std::chrono_literals;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::ResultOf;

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

class WorkflowListenerMock : public SprintTimer::WorkflowListener {
public:
    MOCK_METHOD(void, onTimerTick, (std::chrono::seconds), (override));

    MOCK_METHOD(void, onIdle, (), (override));

    MOCK_METHOD(void, onSprintStarted, (std::chrono::seconds), (override));

    MOCK_METHOD(void, onSprintFinished, (std::vector<Sprint>&&), (override));

    MOCK_METHOD(void, onBreakStarted, (std::chrono::seconds), (override));

    MOCK_METHOD(void, onBreakFinished, (), (override));

    MOCK_METHOD(void, onZoneEntered, (), (override));

    MOCK_METHOD(void, onZoneLeft, (), (override));
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

class WorkflowFixture : public ::testing::Test {
public:
    NiceMock<CountdownTimerMock> countdownTimer;
    SprintTimer sut{countdownTimer, std::chrono::seconds{1}, workflowParams};
    NiceMock<WorkflowListenerMock> listener;
    DateTime someTime{DateTime{Date{Year{2024}, Month{12}, Day{2}}} + 15h +
                      29min};

    auto SetUp() -> void override { sut.addListener(&listener); }

    auto TearDown() -> void override { sut.removeListener(&listener); }

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

    auto expect_no_calls_to_timer()
    {
        EXPECT_CALL(countdownTimer, runImpl(_, _, _, _)).Times(0);
        EXPECT_CALL(countdownTimer, stopImpl()).Times(0);
        EXPECT_CALL(countdownTimer, setLoopImpl(_)).Times(0);
    }
};

TEST_F(WorkflowFixture,
       should_transition_from_idle_to_sprint_state_on_start_event)
{
    EXPECT_CALL(listener, onSprintStarted(sprintDuration));
    EXPECT_CALL(countdownTimer,
                runImpl(testing::ResultOf(convertDuration, sprintDuration),
                        testing::ResultOf(convertTickPeriod, tickPeriod),
                        _,
                        _));

    sut.process(SprintTimer::StartEvent{someTime});
}

TEST_F(WorkflowFixture,
       should_transition_from_sprint_to_finished_on_timer_runout_event)
{
    sut.process(SprintTimer::StartEvent{someTime});

    EXPECT_CALL(listener,
                onSprintFinished(std::vector<Sprint>{
                    Sprint{someTime, sprintDurationInMinutes}}));

    sut.process(SprintTimer::TimerFinishedEvent{});
}

TEST_F(WorkflowFixture, should_transition_from_finished_to_break)
{
    sut.process(SprintTimer::StartEvent{someTime});
    sut.process(SprintTimer::TimerFinishedEvent{});

    EXPECT_CALL(listener, onBreakStarted(shortBreakDuration));
    EXPECT_CALL(countdownTimer,
                runImpl(ResultOf(convertDuration, shortBreakDuration),
                        ResultOf(convertTickPeriod, tickPeriod),
                        _,
                        _));

    sut.process(SprintTimer::StartEvent{someTime + 55min});
}

TEST_F(WorkflowFixture,
       should_transition_from_finished_to_long_break_when_condition_met)
{
    sut.setNumFinishedSprints(sprintsBeforeBreakStarted - 1);
    sut.process(SprintTimer::StartEvent{someTime});
    sut.process(SprintTimer::TimerFinishedEvent{});

    EXPECT_CALL(listener, onBreakStarted(longBreakDuration));
    EXPECT_CALL(countdownTimer,
                runImpl(ResultOf(convertDuration, longBreakDuration),
                        ResultOf(convertTickPeriod, tickPeriod),
                        _,
                        _));

    sut.process(SprintTimer::StartEvent{someTime + 1h});
}

TEST_F(WorkflowFixture, should_transition_from_break_to_idle)
{
    sut.process(SprintTimer::StartEvent{someTime});
    sut.process(SprintTimer::TimerFinishedEvent{});
    sut.process(SprintTimer::StartEvent{someTime + 12min});

    EXPECT_CALL(listener, onIdle());

    sut.process(SprintTimer::TimerFinishedEvent{});
}

TEST_F(WorkflowFixture, should_transition_to_from_sprint_to_zone_when_toggled)
{
    sut.process(SprintTimer::StartEvent{someTime});

    EXPECT_CALL(countdownTimer, setLoopImpl(true));
    EXPECT_CALL(listener, onZoneEntered());

    sut.process(SprintTimer::ZoneToggledEvent{});
}

TEST_F(WorkflowFixture, should_transition_from_zone_to_sprint_when_toggled)
{
    sut.process(SprintTimer::StartEvent{someTime});
    sut.process(SprintTimer::ZoneToggledEvent{});

    EXPECT_CALL(countdownTimer, setLoopImpl(false));
    EXPECT_CALL(listener, onZoneLeft());

    sut.process(SprintTimer::ZoneToggledEvent{});
}

TEST_F(WorkflowFixture, idle_state_cannot_be_cancelled)
{
    expect_no_calls_to_listener();
    expect_no_calls_to_timer();

    sut.process(SprintTimer::CancelEvent{});
}

TEST_F(WorkflowFixture, zone_state_cannot_be_cancelled)
{
    sut.process(SprintTimer::StartEvent{someTime});
    sut.process(SprintTimer::ZoneToggledEvent{});

    expect_no_calls_to_listener();
    expect_no_calls_to_timer();

    sut.process(SprintTimer::CancelEvent{});
}

TEST_F(WorkflowFixture, sprint_states_transitions_to_idle_when_cancelled)
{
    sut.process(SprintTimer::StartEvent{someTime});

    EXPECT_CALL(countdownTimer, stopImpl());
    EXPECT_CALL(listener, onIdle());

    sut.process(SprintTimer::CancelEvent{});
}

TEST_F(WorkflowFixture, break_state_transitions_to_idle_when_cancelled)
{
    sut.process(SprintTimer::StartEvent{someTime});
    sut.process(SprintTimer::TimerFinishedEvent{});
    sut.process(SprintTimer::StartEvent{someTime + 5min});

    EXPECT_CALL(countdownTimer, stopImpl());
    EXPECT_CALL(listener, onIdle());

    sut.process(SprintTimer::CancelEvent{});
}

TEST_F(WorkflowFixture,
       sprint_finished_state_should_transition_to_idle_when_cancelled)
{
    sut.process(SprintTimer::StartEvent{someTime});
    sut.process(SprintTimer::TimerFinishedEvent{});

    EXPECT_CALL(listener, onIdle());

    sut.process(SprintTimer::CancelEvent{});
}

TEST_F(WorkflowFixture, zone_transition_should_be_ignored_in_idle_state)
{
    expect_only_tick_calls_to_listener();
    expect_no_calls_to_timer();

    sut.process(SprintTimer::ZoneToggledEvent{});
}

TEST_F(WorkflowFixture, zone_transition_should_be_ignored_in_finished_state)
{
    sut.process(SprintTimer::StartEvent{someTime});
    sut.process(SprintTimer::TimerFinishedEvent{});

    expect_only_tick_calls_to_listener();
    expect_no_calls_to_timer();

    sut.process(SprintTimer::ZoneToggledEvent{});
}

TEST_F(WorkflowFixture, zone_transition_should_be_ignored_in_break_state)
{
    sut.process(SprintTimer::StartEvent{someTime});
    sut.process(SprintTimer::TimerFinishedEvent{});
    sut.process(SprintTimer::StartEvent{someTime + 25min});

    expect_only_tick_calls_to_listener();
    expect_no_calls_to_timer();

    sut.process(SprintTimer::ZoneToggledEvent{});
}

TEST_F(WorkflowFixture, zone_mode_accumulates_sprints)
{
    sut.process(SprintTimer::StartEvent{someTime});
    sut.process(SprintTimer::ZoneToggledEvent{});
    sut.process(SprintTimer::TimerFinishedEvent{});
    sut.process(SprintTimer::TimerFinishedEvent{});
    sut.process(SprintTimer::ZoneToggledEvent{});

    EXPECT_CALL(
        listener,
        onSprintFinished(testing::ElementsAre(
            Sprint{someTime, sprintDurationInMinutes},
            Sprint{someTime + sprintDurationInMinutes, sprintDurationInMinutes},
            Sprint{someTime + 2 * sprintDurationInMinutes,
                   sprintDurationInMinutes})));

    sut.process(SprintTimer::TimerFinishedEvent{});
}

TEST_F(WorkflowFixture, handles_reconfiguration)
{
    constexpr SprintTimer::WorkflowParams changedConfiguration{
        .sprintDuration = 55min,
        .shortBreakDuration = 20min,
        .longBreakDuration = 30min,
        .numSprintsBeforeLongBreak = 3};
    sut.setNumFinishedSprints(1);
    sut.reconfigure(changedConfiguration);
    sut.process(SprintTimer::StartEvent{someTime + 2h});

    EXPECT_CALL(
        listener,
        onSprintFinished(::testing::ElementsAre(Sprint{someTime + 2h, 55min})));
    sut.process(SprintTimer::TimerFinishedEvent{});

    EXPECT_CALL(listener, onBreakStarted(1200s));
    sut.process(SprintTimer::StartEvent{someTime + 2h + 55min});
    sut.process(SprintTimer::TimerFinishedEvent{});

    sut.process(SprintTimer::StartEvent{someTime + 4h});
    EXPECT_CALL(
        listener,
        onSprintFinished(::testing::ElementsAre(Sprint{someTime + 4h, 55min})));
    sut.process(SprintTimer::TimerFinishedEvent{});

    EXPECT_CALL(listener, onBreakStarted(1800s));
    sut.process(SprintTimer::StartEvent{someTime + 5h});
}
