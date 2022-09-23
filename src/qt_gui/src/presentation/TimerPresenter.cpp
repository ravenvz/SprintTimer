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
#include <string_view>

namespace {

constexpr std::string_view sprintColor{"#eb6c59"};
constexpr std::string_view breakColor{"#73c245"};
constexpr std::string_view zoneColor{"#808000"};
constexpr std::string_view idleTimerText{"Start"};
constexpr std::string_view submissionTimerText{"Submit"};

} // namespace

namespace sprint_timer::ui {

TimerPresenter::TimerPresenter(IWorkflow& workflow_,
                               today_progress_hdl_t& todayProgressHandler_,
                               api::SoundPlayer& player_,
                               const AssetLibrary& assetLibrary_,
                               std::string ringSoundId_,
                               TaskSelectionMediator& taskSelectionMediator_)
    : workflow{workflow_}
    , todayProgressHandler{todayProgressHandler_}
    , player{player_}
    , assetLibrary{assetLibrary_}
    , ringSoundId{std::move(ringSoundId_)}
    , taskSelectionMediator{taskSelectionMediator_}
{
    taskSelectionMediator.addColleague(this);
    workflow.addListener(this);
}

TimerPresenter::~TimerPresenter()
{
    taskSelectionMediator.removeColleague(this);
    workflow.removeListener(this);
}

void TimerPresenter::updateViewImpl()
{
    if (auto v = view(); v) {
        const auto progress =
            todayProgressHandler.handle(api::RequestProgressQuery{});
        if (progress.size() != 0) {
            workflow.setNumFinishedSprints(
                todayProgressHandler.handle(api::RequestProgressQuery{})
                    .getValue(progress.size() - 1)
                    .actual());
        }
        v.value()->setupUi(contracts::TimerContract::TimerUiModel::idleUiModel(
            std::string{idleTimerText}));
    }
}

void TimerPresenter::onViewAttached() { updateView(); }

void TimerPresenter::onTimerTick(std::chrono::seconds timeLeft)
{
    if (auto v = view(); v) {
        v.value()->updateTimerValue(timeLeft);
    }
}

void TimerPresenter::onWorkflowStateChanged(IWorkflow::StateId currentState)
{
    utils::inspect(view(), [&](auto* view) {
        using contracts::TimerContract::TimerUiModel;
        switch (currentState) {
        case IWorkflow::StateId::Idle:
            view->setupUi(
                TimerUiModel::idleUiModel(std::string{idleTimerText}));
            break;
        case IWorkflow::StateId::RunningSprint:
            view->setupUi(TimerUiModel::runningUiModel(
                workflow.currentDuration(), std::string{sprintColor}, true));
            break;
        case IWorkflow::StateId::SprintFinished:
            if (auto mediaPath = assetLibrary.filePath(ringSoundId);
                mediaPath) {
                player.play(*mediaPath);
            }
            view->selectTask(taskSelectionMediator.taskUuid());
            view->setupUi(TimerUiModel::sprintFinishedUiModel(
                std::string{submissionTimerText}));
            break;
        case IWorkflow::StateId::BreakStarted:
            view->setupUi(TimerUiModel::runningUiModel(
                workflow.currentDuration(), std::string{breakColor}, false));
            break;
        case IWorkflow::StateId::BreakFinished:
            player.play(*assetLibrary.filePath(ringSoundId));
            break;
        case IWorkflow::StateId::ZoneEntered:
            view->setupUi(
                TimerUiModel::zoneModeUiModel(std::string{zoneColor}));
            break;
        case IWorkflow::StateId::ZoneLeft:
            view->setupUi(
                TimerUiModel::returnFromZoneUiModel(std::string{sprintColor}));
            break;
        }
    });
}

void TimerPresenter::onTimerClicked()
{
    const auto& timeRanges = workflow.completedSprints();
    auto v = view();

    if (v && !timeRanges.empty()) {
        v.value()->submitSprints(timeRanges);
    }
    workflow.start();
}

void TimerPresenter::onCancelClicked() { workflow.cancel(); }

void TimerPresenter::onZoneClicked() { workflow.toggleInTheZoneMode(); }

void TimerPresenter::onTaskSelectionChanged()
{
    utils::inspect(view(), [&](auto* view) {
        view->selectTask(taskSelectionMediator.taskUuid());
    });
}

void TimerPresenter::changeTaskSelection(api::TaskDTO&& task)
{
    taskSelectionMediator.changeSelection(this, std::move(task));
}

} // namespace sprint_timer::ui
