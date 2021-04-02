/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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
#ifndef TIMERPRESENTER_H_O4HCGZN9
#define TIMERPRESENTER_H_O4HCGZN9

#include "qt_gui/presentation/AssetLibrary.h"
#include "qt_gui/presentation/TaskSelectionMediator.h"
#include "qt_gui/presentation/TimerContract.h"
#include <core/CommandHandler.h>
#include <core/IWorkflow.h>
#include <core/SoundPlayer.h>
#include <core/use_cases/register_sprint/RegisterSprintBulkCommand.h>
#include <core/use_cases/workflow_control/CancelTimer.h>
#include <core/use_cases/workflow_control/StartTimer.h>
#include <core/use_cases/workflow_control/ToggleZoneMode.h>

namespace {

const std::string sprintColor{"#eb6c59"};
const std::string breakColor{"#73c245"};
const std::string zoneColor{"#808000"};
const std::string idleTimerText{"Start"};
const std::string submissionTimerText{"Submit"};

} // namespace

namespace sprint_timer::ui {

class TimerPresenter : public contracts::TimerContract::Presenter,
                       public IWorkflow::WorkflowListener {
public:
    TimerPresenter(IWorkflow& workflow,
                   SoundPlayer& player,
                   const AssetLibrary& assetLibrary,
                   std::string ringSoundId,
                   TaskSelectionMediator& taskSelectionMediator);

    ~TimerPresenter() override;

    void onTimerTick(std::chrono::seconds timeLeft) override;

    void onCancelClicked() override;

    void onZoneClicked() override;

    void onTimerClicked() override;

    void onWorkflowStateChanged(IWorkflow::StateId currentState) override;

    void onTaskSelectionChanged() override;

    void changeTaskSelection(size_t index, std::string&& uuid) override;

private:
    IWorkflow& workflow;
    SoundPlayer& player;
    const AssetLibrary& assetLibrary;
    std::string ringSoundId;
    TaskSelectionMediator& taskSelectionMediator;

    void updateViewImpl() override;

    void onViewAttached() override;
};

inline TimerPresenter::TimerPresenter(
    IWorkflow& workflow_,
    SoundPlayer& player_,
    const AssetLibrary& assetLibrary_,
    std::string ringSoundId_,
    TaskSelectionMediator& taskSelectionMediator_)
    : workflow{workflow_}
    , player{player_}
    , assetLibrary{assetLibrary_}
    , ringSoundId{std::move(ringSoundId_)}
    , taskSelectionMediator{taskSelectionMediator_}
{
    taskSelectionMediator.addColleague(this);
    workflow.addListener(this);
}

inline TimerPresenter::~TimerPresenter()
{
    taskSelectionMediator.removeColleague(this);
    workflow.removeListener(this);
}

inline void TimerPresenter::updateViewImpl()
{
    if (auto v = view(); v) {
        v.value()->setupUi(
            contracts::TimerContract::TimerUiModel::idleUiModel(idleTimerText));
    }
}

inline void TimerPresenter::onViewAttached() { updateView(); }

inline void TimerPresenter::onTimerTick(std::chrono::seconds timeLeft)
{
    if (auto v = view(); v) {
        v.value()->updateTimerValue(timeLeft);
    }
}

inline void
TimerPresenter::onWorkflowStateChanged(IWorkflow::StateId currentState)
{
    if (auto v = view(); v) {
        using contracts::TimerContract::TimerUiModel;
        switch (currentState) {
        case IWorkflow::StateId::Idle:
            v.value()->setupUi(TimerUiModel::idleUiModel(idleTimerText));
            break;
        case IWorkflow::StateId::RunningSprint:
            v.value()->setupUi(TimerUiModel::runningUiModel(
                workflow.currentDuration(), sprintColor, true));
            break;
        case IWorkflow::StateId::SprintFinished:
            player.play(*assetLibrary.filePath(ringSoundId));
            v.value()->setupUi(
                TimerUiModel::sprintFinishedUiModel(submissionTimerText));
            break;
        case IWorkflow::StateId::BreakStarted:
            v.value()->setupUi(TimerUiModel::runningUiModel(
                workflow.currentDuration(), breakColor, false));
            break;
        case IWorkflow::StateId::BreakFinished:
            player.play(*assetLibrary.filePath(ringSoundId));
            break;
        case IWorkflow::StateId::ZoneEntered:
            v.value()->setupUi(TimerUiModel::zoneModeUiModel(zoneColor));
            break;
        case IWorkflow::StateId::ZoneLeft:
            v.value()->setupUi(
                TimerUiModel::returnFromZoneUiModel(sprintColor));
            break;
        }
    }
}

inline void TimerPresenter::onTimerClicked()
{
    const auto& timeRanges = workflow.completedSprints();
    auto v = view();

    if (v && !timeRanges.empty()) {
        v.value()->submitSprints(timeRanges);
    }
    workflow.start();
}

inline void TimerPresenter::onCancelClicked() { workflow.cancel(); }

inline void TimerPresenter::onZoneClicked() { workflow.toggleInTheZoneMode(); }

inline void TimerPresenter::onTaskSelectionChanged()
{
    auto v = view();
    if (!v) {
        return;
    }
    if (auto index = taskSelectionMediator.taskIndex(); index) {
        v.value()->selectTask(*index);
    }
}

inline void TimerPresenter::changeTaskSelection(size_t index,
                                                std::string&& uuid)
{
    taskSelectionMediator.changeSelection(this, index, std::move(uuid));
}

} // namespace sprint_timer::ui

#endif /* end of include guard: TIMERPRESENTER_H_O4HCGZN9 */
