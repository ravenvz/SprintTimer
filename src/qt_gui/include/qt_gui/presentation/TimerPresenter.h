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
#ifndef TIMERPRESENTER_H_O4HCGZN9
#define TIMERPRESENTER_H_O4HCGZN9

#include "qt_gui/presentation/AssetLibrary.h"
#include "qt_gui/presentation/TaskSelectionMediator.h"
#include "qt_gui/presentation/TimerContract.h"
#include "core/CommandHandler.h"
#include "core/IWorkflow.h"
#include "core/SoundPlayer.h"
#include "core/use_cases/register_sprint/RegisterSprintBulkCommand.h"
#include "core/use_cases/workflow_control/CancelTimer.h"
#include "core/use_cases/workflow_control/StartTimer.h"
#include "core/use_cases/workflow_control/ToggleZoneMode.h"

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

} // namespace sprint_timer::ui

#endif /* end of include guard: TIMERPRESENTER_H_O4HCGZN9 */
