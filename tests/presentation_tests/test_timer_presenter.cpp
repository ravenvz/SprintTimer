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
#include "core/entities/Task.h"
#include "core/use_cases/workflow_control/StartTimer.h"
#include "mocks/AssetLibraryMock.h"
#include "mocks/QueryHandlerMock.h"
#include "mocks/SoundPlayerMock.h"
#include "qt_gui/presentation/TimerPresenter.h"
#include "gmock/gmock.h"

using sprint_timer::IWorkflow;
using sprint_timer::ui::TimerPresenter;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

using namespace sprint_timer::ui::contracts::TimerContract;
using namespace sprint_timer::use_cases;

namespace {

constexpr std::string_view sprintStateColor{"#eb6c59"};
constexpr std::string_view brakeStateColor{"#73c245"};
constexpr std::string_view zoneStateColor{"#808000"};
constexpr std::string_view ringSoundId{"ringSoundId"};

} // namespace

namespace sprint_timer::ui::contracts::TimerContract {

bool operator==(const TimerUiModel& lhs, const TimerUiModel& rhs)
{
    return lhs.submissionBoxVisible == rhs.submissionBoxVisible &&
           lhs.cancelControlVisible == rhs.cancelControlVisible &&
           lhs.zoneControlVisible == rhs.zoneControlVisible &&
           lhs.dialVisible == rhs.dialVisible &&
           lhs.invertedDial == rhs.invertedDial &&
           lhs.dialColor == rhs.dialColor && lhs.centerText == rhs.centerText &&
           lhs.timerTotalDuration == rhs.timerTotalDuration;
}

} // namespace sprint_timer::ui::contracts::TimerContract

class TimerViewMock : public View {
public:
    MOCK_METHOD(void, updateTimerValue, (std::chrono::seconds), (override));

    MOCK_METHOD(void, setupUi, (TimerUiModel &&), (override));

    MOCK_METHOD(void,
                submitSprints,
                (const std::vector<dw::DateTimeRange>&),
                (override));

    MOCK_METHOD(void, selectTask, (size_t), (override));
};

class WorkflowTMock : public sprint_timer::IWorkflow {
public:
    MOCK_METHOD(void, start, (), (override));

    MOCK_METHOD(void, cancel, (), (override));

    MOCK_METHOD(std::chrono::seconds, currentDuration, (), (const override));

    MOCK_METHOD(void, toggleInTheZoneMode, (), (override));

    MOCK_METHOD(std::vector<dw::DateTimeRange>,
                completedSprints,
                (),
                (const override));

    MOCK_METHOD(void, setNumFinishedSprints, (int), (override));

    MOCK_METHOD(void, addListener, (IWorkflow::WorkflowListener*), (override));

    MOCK_METHOD(void,
                removeListener,
                (IWorkflow::WorkflowListener*),
                (override));

    MOCK_METHOD(void,
                reconfigure,
                (const IWorkflow::WorkflowParams&),
                (override));
};

class TimerPresenterFixture : public ::testing::Test {
public:
    TimerViewMock view;
    NiceMock<WorkflowTMock> workflow;
    NiceMock<
        mocks::QueryHandlerMock<sprint_timer::use_cases::RequestProgressQuery>>
        todayProgressHandler;
    const std::string someSoundPath{"somesoundpath"};
    NiceMock<mocks::SoundPlayerMock> soundPlayer;
    NiceMock<mocks::AssetLibraryMock> assetLibrary;
    sprint_timer::ui::TaskSelectionMediator taskSelectionMediator;
    TimerPresenter presenter{workflow,
                             todayProgressHandler,
                             soundPlayer,
                             assetLibrary,
                             std::string{ringSoundId},
                             taskSelectionMediator};

    void SetUp() override
    {
        using sprint_timer::GoalProgress;
        ON_CALL(todayProgressHandler, handle(_))
            .WillByDefault(
                Return(sprint_timer::ProgressOverPeriod{{GoalProgress{
                    GoalProgress::Estimated{12}, GoalProgress::Actual{7}}}}));
    }
};

TEST_F(TimerPresenterFixture, upon_creation_starts_listening_workflow)
{
    EXPECT_CALL(workflow, addListener(_));

    TimerPresenter otherPresenter{workflow,
                                  todayProgressHandler,
                                  soundPlayer,
                                  assetLibrary,
                                  std::string{ringSoundId},
                                  taskSelectionMediator};
}

TEST_F(TimerPresenterFixture, removes_self_as_workflow_listener_when_destroyed)
{
    NiceMock<WorkflowTMock> wflow;
    EXPECT_CALL(wflow, removeListener(_));

    {
        TimerPresenter otherPresenter{wflow,
                                      todayProgressHandler,
                                      soundPlayer,
                                      assetLibrary,
                                      std::string{ringSoundId},
                                      taskSelectionMediator};
    }
}

TEST_F(TimerPresenterFixture, sets_up_idle_view_state_when_view_is_attached)
{
    const std::string text{"Start"};

    EXPECT_CALL(view, setupUi(TimerUiModel::idleUiModel(text)));

    presenter.attachView(view);
}

TEST_F(TimerPresenterFixture,
       sets_sprint_count_for_workflow_when_view_is_attached)
{
    using sprint_timer::GoalProgress;
    NiceMock<
        mocks::QueryHandlerMock<sprint_timer::use_cases::RequestProgressQuery>>
        progressHandler;
    const int numActualSprints{4};
    TimerPresenter otherPresenter{workflow,
                                  progressHandler,
                                  soundPlayer,
                                  assetLibrary,
                                  std::string{ringSoundId},
                                  taskSelectionMediator};
    ON_CALL(progressHandler, handle(_))
        .WillByDefault(Return(sprint_timer::ProgressOverPeriod{
            {GoalProgress{GoalProgress::Estimated{14}, GoalProgress::Actual{2}},
             GoalProgress{GoalProgress::Estimated{12},
                          GoalProgress::Actual{numActualSprints}}}}));
    EXPECT_CALL(workflow, setNumFinishedSprints(numActualSprints));

    otherPresenter.attachView(view);
}

TEST_F(TimerPresenterFixture, updates_timer_value)
{
    const std::chrono::seconds secondsLeft{58};
    presenter.attachView(view);

    EXPECT_CALL(view, updateTimerValue(secondsLeft));

    presenter.onTimerTick(secondsLeft);
}

TEST_F(TimerPresenterFixture, displays_idle_ui_when_workflow_enters_idle_state)
{
    presenter.attachView(view);

    EXPECT_CALL(view, setupUi(TimerUiModel::idleUiModel("Start")));

    presenter.onWorkflowStateChanged(IWorkflow::StateId::Idle);
}

TEST_F(TimerPresenterFixture, displays_submission_ui_when_sprint_ends)
{
    const size_t taskIndex{2};
    std::string taskUuid{"123"};
    taskSelectionMediator.changeSelection(
        nullptr, taskIndex, std::move(taskUuid));
    presenter.attachView(view);

    EXPECT_CALL(view, selectTask(taskIndex));
    EXPECT_CALL(view, setupUi(TimerUiModel::sprintFinishedUiModel("Submit")));

    presenter.onWorkflowStateChanged(IWorkflow::StateId::SprintFinished);
}

TEST_F(TimerPresenterFixture, displays_running_state_when_sprint_started)
{
    const std::chrono::seconds sprintDuration{1500};
    ON_CALL(workflow, currentDuration()).WillByDefault(Return(sprintDuration));
    presenter.attachView(view);

    EXPECT_CALL(view,
                setupUi(TimerUiModel::runningUiModel(
                    sprintDuration, std::string{sprintStateColor}, true)));

    presenter.onWorkflowStateChanged(IWorkflow::StateId::RunningSprint);
}

TEST_F(TimerPresenterFixture, displays_running_state_when_brake_started)
{
    std::chrono::seconds breakDuration{300};
    presenter.attachView(view);
    ON_CALL(workflow, currentDuration()).WillByDefault(Return(breakDuration));

    EXPECT_CALL(view,
                setupUi(TimerUiModel::runningUiModel(
                    breakDuration, std::string{brakeStateColor}, false)));

    presenter.onWorkflowStateChanged(IWorkflow::StateId::BreakStarted);
}

TEST_F(TimerPresenterFixture,
       when_timer_clicked_starts_workflow_when_timer_in_idle)
{
    EXPECT_CALL(workflow, start());

    presenter.onTimerClicked();
}

TEST_F(
    TimerPresenterFixture,
    when_timer_clicked_in_submission_mode_orders_view_to_submit_sprints_and_starts_workflow)
{
    using namespace std::chrono_literals;
    presenter.attachView(view);
    dw::DateTimeRange someInterval{dw::current_date_time(),
                                   dw::current_date_time() + 25min};
    const std::vector<dw::DateTimeRange> timeIntervals{
        someInterval,
        dw::add_offset(someInterval, 25min),
        dw::add_offset(someInterval, 50min),
        dw::add_offset(someInterval, 75min)};
    presenter.onWorkflowStateChanged(IWorkflow::StateId::SprintFinished);
    ON_CALL(workflow, completedSprints()).WillByDefault(Return(timeIntervals));

    EXPECT_CALL(view, submitSprints(timeIntervals));
    EXPECT_CALL(workflow, start());

    presenter.onTimerClicked();
}

TEST_F(TimerPresenterFixture, cancels_workflow_state_when_cancel_clicked)
{
    EXPECT_CALL(workflow, cancel());

    presenter.onCancelClicked();
}

TEST_F(TimerPresenterFixture, delegates_to_workflow_when_zone_button_clicked)
{
    EXPECT_CALL(workflow, toggleInTheZoneMode());

    presenter.onZoneClicked();
}

TEST_F(TimerPresenterFixture, switches_view_into_the_zone_mode)
{
    presenter.attachView(view);

    EXPECT_CALL(
        view,
        setupUi(TimerUiModel::zoneModeUiModel(std::string{zoneStateColor})));

    presenter.onWorkflowStateChanged(IWorkflow::StateId::ZoneEntered);
}

TEST_F(TimerPresenterFixture, switches_view_out_of_the_zone_mode)
{
    presenter.attachView(view);

    EXPECT_CALL(view,
                setupUi(TimerUiModel::returnFromZoneUiModel(
                    std::string{sprintStateColor})));

    presenter.onWorkflowStateChanged(IWorkflow::StateId::ZoneLeft);
}

TEST_F(TimerPresenterFixture, plays_sound_when_break_finished)
{
    ON_CALL(assetLibrary, filePath(std::string{ringSoundId}))
        .WillByDefault(Return(someSoundPath));
    presenter.attachView(view);

    EXPECT_CALL(soundPlayer, play(someSoundPath));

    presenter.onWorkflowStateChanged(IWorkflow::StateId::BreakFinished);
}

TEST_F(TimerPresenterFixture, plays_sound_when_sprint_finished)
{
    ON_CALL(assetLibrary, filePath(std::string{ringSoundId}))
        .WillByDefault(Return(someSoundPath));
    presenter.attachView(view);

    EXPECT_CALL(soundPlayer, play(someSoundPath));

    presenter.onWorkflowStateChanged(IWorkflow::StateId::SprintFinished);
}
