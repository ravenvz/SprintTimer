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

#include "api/SoundPlayer.h"
#include "api/com_query/CommandHandler.h"
#include "api/com_query/QueryHandler.h"
#include "core/IWorkflow.h"
// #include "api/requests/RegisterSprintBulkCommand.h"
#include "api/requests/RequestProgressQuery.h"
// #include "api/requests/CancelTimer.h"
// #include "api/requests/StartTimer.h"
// #include "api/requests/ToggleZoneMode.h"
#include "qt_gui/presentation/AssetLibrary.h"
#include "qt_gui/presentation/TaskSelectionMediator.h"
#include "qt_gui/presentation/TimerContract.h"

namespace sprint_timer::ui {

class TimerPresenter : public contracts::TimerContract::Presenter,
                       public IWorkflow::WorkflowListener {
public:
    using today_progress_hdl_t = asp::QueryHandler<api::RequestProgressQuery>;

    TimerPresenter(IWorkflow& workflow,
                   today_progress_hdl_t& todayProgressHandler,
                   api::SoundPlayer& player,
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

    void changeTaskSelection(api::TaskDTO&& task) override;

private:
    IWorkflow& workflow;
    today_progress_hdl_t& todayProgressHandler;
    api::SoundPlayer& player;
    const AssetLibrary& assetLibrary;
    std::string ringSoundId;
    TaskSelectionMediator& taskSelectionMediator;

    void updateViewImpl() override;

    void onViewAttached() override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: TIMERPRESENTER_H_O4HCGZN9 */
