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
#include "api/WorkflowService.h"
#include "api/com_query/CommandHandler.h"
#include "api/requests/CancelWorkflowCommand.h"
#include "api/requests/RegisterSprintBulkCommand.h"
#include "api/requests/RequestProgressQuery.h"
#include "api/requests/StartTimerCommand.h"
#include "api/requests/ToggleZoneCommand.h"
#include "qt_gui/presentation/AssetLibrary.h"
#include "qt_gui/presentation/TaskSelectionMediator.h"
#include "qt_gui/presentation/TimerContract.h"

namespace sprint_timer::ui {

class TimerPresenter : public contracts::TimerContract::Presenter,
                       public api::TimerWorkflowListener {
public:
    using start_workflow_hdl_t = asp::CommandHandler<api::StartTimerCommand>;
    using cancel_workflow_hdl_t =
        asp::CommandHandler<api::CancelWorkflowCommand>;
    using toggle_zone_hdl_t = asp::CommandHandler<api::ToggleZoneCommand>;
    using register_sprints_hdl_t =
        asp::CommandHandler<api::RegisterSprintBulkCommand>;

    TimerPresenter(start_workflow_hdl_t& startWorkflowHandler,
                   cancel_workflow_hdl_t& cancelWorkflowHandler,
                   toggle_zone_hdl_t& toggleZoneHandler,
                   register_sprints_hdl_t& registerSprintHandler,
                   api::SoundPlayer& player,
                   const AssetLibrary& assetLibrary,
                   std::string ringSoundId,
                   TaskSelectionMediator& taskSelectionMediator);

    ~TimerPresenter() override;

    void onCancelClicked() override;

    void onZoneClicked() override;

    void onTimerClicked() override;

    void onTaskSelectionChanged() override;

    void changeTaskSelection(api::TaskDTO&& task) override;

    auto onIdle() -> void override;

    auto onSprintStarted(std::chrono::seconds timeLeft) -> void override;

    auto onSprintFinished(std::vector<dw::DateTimeRange>&&) -> void override;

    auto onBreakStarted(std::chrono::seconds timeLeft) -> void override;

    auto onBreakFinished() -> void override;

    auto onZoneEntered() -> void override;

    auto onZoneLeft() -> void override;

    auto onTimerTick(std::chrono::seconds timeLeft) -> void override;

private:
    start_workflow_hdl_t& startWorkflowHandler;
    cancel_workflow_hdl_t& cancelWorkflowHandler;
    toggle_zone_hdl_t& toggleZoneHandler;
    register_sprints_hdl_t& registerSprintHandler;
    api::SoundPlayer& player;
    const AssetLibrary& assetLibrary;
    std::string ringSoundId;
    TaskSelectionMediator& taskSelectionMediator;
    std::vector<dw::DateTimeRange> pendingSprints;

    void updateViewImpl() override;

    void onViewAttached() override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: TIMERPRESENTER_H_O4HCGZN9 */
