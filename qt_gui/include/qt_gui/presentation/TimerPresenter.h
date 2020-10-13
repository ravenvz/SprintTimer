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
                   std::string ringSoundId);

    ~TimerPresenter() override;

    void onTimerTick(std::chrono::seconds timeLeft) override;

    void onCancelClicked() override;

    void onZoneClicked() override;

    void onTimerClicked() override;

    void onWorkflowStateChanged(IWorkflow::StateId currentState) override;

private:
    IWorkflow& workflow;
    SoundPlayer& player;
    const AssetLibrary& assetLibrary;
    std::string ringSoundId;

    void updateViewImpl() override;
};

inline TimerPresenter::TimerPresenter(IWorkflow& workflow_,
                                      SoundPlayer& player_,
                                      const AssetLibrary& assetLibrary_,
                                      std::string ringSoundId_)
    : workflow{workflow_}
    , player{player_}
    , assetLibrary{assetLibrary_}
    , ringSoundId{std::move(ringSoundId_)}
{
    workflow.addListener(this);
}

inline TimerPresenter::~TimerPresenter() { workflow.removeListener(this); }

inline void TimerPresenter::updateViewImpl()
{
    using contracts::TimerContract::TimerUiModel;
    view->setupUi(TimerUiModel::idleUiModel(idleTimerText));
}

inline void TimerPresenter::onTimerTick(std::chrono::seconds timeLeft)
{
    view->updateTimerValue(timeLeft);
}

inline void
TimerPresenter::onWorkflowStateChanged(IWorkflow::StateId currentState)
{
    using contracts::TimerContract::TimerUiModel;
    switch (currentState) {
    case IWorkflow::StateId::Idle:
        view->setupUi(TimerUiModel::idleUiModel(idleTimerText));
        break;
    case IWorkflow::StateId::RunningSprint:
        view->setupUi(TimerUiModel::runningUiModel(
            workflow.currentDuration(), sprintColor, true));
        break;
    case IWorkflow::StateId::SprintFinished:
        player.play(*assetLibrary.filePath(ringSoundId));
        view->setupUi(TimerUiModel::sprintFinishedUiModel(submissionTimerText));
        break;
    case IWorkflow::StateId::BreakStarted:
        view->setupUi(TimerUiModel::runningUiModel(
            workflow.currentDuration(), breakColor, false));
        break;
    case IWorkflow::StateId::BreakFinished:
        player.play(*assetLibrary.filePath(ringSoundId));
        break;
    case IWorkflow::StateId::ZoneEntered:
        view->setupUi(TimerUiModel::zoneModeUiModel(zoneColor));
        break;
    case IWorkflow::StateId::ZoneLeft:
        view->setupUi(TimerUiModel::returnFromZoneUiModel(sprintColor));
        break;
    }
}

inline void TimerPresenter::onTimerClicked()
{
    if (const auto& timeRanges = workflow.completedSprints();
        !timeRanges.empty()) {
        view->submitSprints(timeRanges);
    }
    workflow.start();
}

inline void TimerPresenter::onCancelClicked() { workflow.cancel(); }

inline void TimerPresenter::onZoneClicked() { workflow.toggleInTheZoneMode(); }

} // namespace sprint_timer::ui

#endif /* end of include guard: TIMERPRESENTER_H_O4HCGZN9 */
