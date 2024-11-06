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
#include "qt_gui/presentation/TimerPresenter.h"
#include "api/requests/CancelWorkflowCommand.h"
#include "api/requests/StartTimerCommand.h"
#include "api/requests/ToggleZoneCommand.h"
#include "cpp_utils/algorithms/optional_ext.h"
#include <string_view>

namespace {

constexpr std::string_view sprintColor{"#eb6c59"};
constexpr std::string_view breakColor{"#73c245"};
constexpr std::string_view zoneColor{"#808000"};
constexpr std::string_view idleTimerText{"Start"};
constexpr std::string_view submissionTimerText{"Submit"};

} // namespace

namespace sprint_timer::ui {

TimerPresenter::TimerPresenter(start_workflow_hdl_t& startWorkflowHandler_,
                               cancel_workflow_hdl_t& cancelWorkflowHandler_,
                               toggle_zone_hdl_t& toggleZoneHandler_,
                               register_sprints_hdl_t& registerSprintHandler_,
                               api::SoundPlayer& player_,
                               const AssetLibrary& assetLibrary_,
                               std::string ringSoundId_,
                               TaskSelectionMediator& taskSelectionMediator_)
    : startWorkflowHandler{startWorkflowHandler_}
    , cancelWorkflowHandler{cancelWorkflowHandler_}
    , toggleZoneHandler{toggleZoneHandler_}
    , registerSprintHandler{registerSprintHandler_}
    , player{player_}
    , assetLibrary{assetLibrary_}
    , ringSoundId{std::move(ringSoundId_)}
    , taskSelectionMediator{taskSelectionMediator_}
{
    taskSelectionMediator.addColleague(this);
}

TimerPresenter::~TimerPresenter()
{
    taskSelectionMediator.removeColleague(this);
}

void TimerPresenter::updateViewImpl()
{
    alg::inspect(view(), [&](auto* view) {
        view->setupUi(contracts::TimerContract::TimerUiModel::idleUiModel(
            std::string{idleTimerText}));
    });
}

void TimerPresenter::onViewAttached() { updateView(); }

void TimerPresenter::onTimerClicked()
{
    if (not pendingSprints.empty() and taskSelectionMediator.taskUuid()) {
        registerSprintHandler.handle(api::RegisterSprintBulkCommand{
            *taskSelectionMediator.taskUuid(), pendingSprints});
    }
    startWorkflowHandler.handle(api::StartTimerCommand{});
    pendingSprints.clear();
    // const auto& timeRanges = workflow.completedSprints();
    // auto v = view();
    //
    // if (v && !timeRanges.empty()) {
    //     v.value()->submitSprints(timeRanges);
    // }
    // workflow.process(TimerWorkflow::StartEvent{});
}

void TimerPresenter::onCancelClicked()
{
    pendingSprints.clear();
    cancelWorkflowHandler.handle(api::CancelWorkflowCommand{});
}

void TimerPresenter::onZoneClicked()
{
    toggleZoneHandler.handle(api::ToggleZoneCommand{});
}

void TimerPresenter::onTaskSelectionChanged()
{
    alg::inspect(view(), [&](auto* view) {
        view->selectTask(taskSelectionMediator.taskUuid());
    });
}

void TimerPresenter::changeTaskSelection(api::TaskDTO&& task)
{
    taskSelectionMediator.changeSelection(this, std::move(task));
}

// void TimerPresenter::onWorkflowStateChanged(TimerWorkflow::StateId
// currentState)
// {
//     alg::inspect(view(), [&](auto* view) {
//         using contracts::TimerContract::TimerUiModel;
//         switch (currentState) {
//         case TimerWorkflow::StateId::Idle:
//             view->setupUi(
//                 TimerUiModel::idleUiModel(std::string{idleTimerText}));
//             break;
//         case TimerWorkflow::StateId::RunningSprint:
//             view->setupUi(TimerUiModel::runningUiModel(
//                 workflow.currentDuration(), std::string{sprintColor}, true));
//             break;
//         case TimerWorkflow::StateId::SprintFinished:
//             if (auto mediaPath = assetLibrary.filePath(ringSoundId);
//                 mediaPath) {
//                 player.play(*mediaPath);
//             }
//             view->selectTask(taskSelectionMediator.taskUuid());
//             view->setupUi(TimerUiModel::sprintFinishedUiModel(
//                 std::string{submissionTimerText}));
//             break;
//         case TimerWorkflow::StateId::BreakStarted:
//             view->setupUi(TimerUiModel::runningUiModel(
//                 workflow.currentDuration(), std::string{breakColor}, false));
//             break;
//         case TimerWorkflow::StateId::BreakFinished:
//             player.play(*assetLibrary.filePath(ringSoundId));
//             break;
//         case TimerWorkflow::StateId::ZoneEntered:
//             view->setupUi(
//                 TimerUiModel::zoneModeUiModel(std::string{zoneColor}));
//             break;
//         case TimerWorkflow::StateId::ZoneLeft:
//             view->setupUi(
//                 TimerUiModel::returnFromZoneUiModel(std::string{sprintColor}));
//             break;
//         }
//     });
// }

auto TimerPresenter::onIdle() -> void
{
    alg::inspect(view(), [&](auto* view) {
        view->setupUi(contracts::TimerContract::TimerUiModel::idleUiModel(
            std::string{idleTimerText}));
    });
}

auto TimerPresenter::onSprintStarted(std::chrono::seconds timeLeft) -> void
{
    alg::inspect(view(), [&](auto* view) {
        view->setupUi(contracts::TimerContract::TimerUiModel::runningUiModel(
            timeLeft, std::string{sprintColor}, true));
    });
}

auto TimerPresenter::onSprintFinished(std::vector<dw::DateTimeRange>&& sprints)
    -> void
{
    alg::inspect(view(), [&](auto* view) {
        view->selectTask(taskSelectionMediator.taskUuid());
        if (auto mediaPath = assetLibrary.filePath(ringSoundId); mediaPath) {
            player.play(*mediaPath);
        }
        view->setupUi(
            contracts::TimerContract::TimerUiModel::sprintFinishedUiModel(
                std::string{submissionTimerText}));
        pendingSprints = std::move(sprints);
    });
}

auto TimerPresenter::onBreakStarted(std::chrono::seconds timeLeft) -> void
{
    alg::inspect(view(), [&](auto* view) {
        view->setupUi(contracts::TimerContract::TimerUiModel::runningUiModel(
            timeLeft, std::string{breakColor}, false));
    });
}

auto TimerPresenter::onBreakFinished() -> void
{
    if (auto mediaPath = assetLibrary.filePath(ringSoundId); mediaPath) {
        player.play(*mediaPath);
    }
}

auto TimerPresenter::onZoneEntered() -> void
{

    alg::inspect(view(), [&](auto* view) {
        view->setupUi(contracts::TimerContract::TimerUiModel::zoneModeUiModel(
            std::string{zoneColor}));
    });
}

auto TimerPresenter::onZoneLeft() -> void
{
    alg::inspect(view(), [&](auto* view) {
        view->setupUi(
            contracts::TimerContract::TimerUiModel::returnFromZoneUiModel(
                std::string{sprintColor}));
    });
}

void TimerPresenter::onTimerTick(std::chrono::seconds timeLeft)
{
    if (auto v = view(); v) {
        v.value()->updateTimerValue(timeLeft);
    }
}

} // namespace sprint_timer::ui
