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
#ifndef HISTORYPRESENTER_H_FAVPGZSN
#define HISTORYPRESENTER_H_FAVPGZSN

#include "core/QueryHandler.h"
#include "core/use_cases/request_sprints/RequestSprintsQuery.h"
#include "core/use_cases/request_tasks/FinishedTasksQuery.h"
#include "qt_gui/presentation/HistoryContract.h"
#include "qt_gui/presentation/HistoryMediator.h"
#include <optional>

namespace sprint_timer::ui {

class HistoryPresenter : public contracts::HistoryContract::Presenter,
                         public HistoryColleague {
public:
    using request_sprints_hdl_t = QueryHandler<use_cases::RequestSprintsQuery>;
    using finished_task_hdl_t = QueryHandler<use_cases::FinishedTasksQuery>;

    HistoryPresenter(request_sprints_hdl_t& requestSprintsHandler,
                     finished_task_hdl_t& requestTasksHandler,
                     HistoryMediator& mediator);

    ~HistoryPresenter() override;

    void onEditTaskMenuSelected(const std::string& uuid) const override;

    void onEditSprintMenuSelected(const std::string& uuid) const override;

    void onDisplayedTabChanged(int tabNumber) override;

    void onSharedDataChanged() override;

private:
    request_sprints_hdl_t& requestSprintsHandler;
    finished_task_hdl_t& requestTasksHandler;
    HistoryMediator& mediator;
    std::optional<request_sprints_hdl_t::result_t> sprintData;
    std::optional<finished_task_hdl_t::result_t> taskData;

    void fetchDataImpl() override;

    void updateViewImpl() override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: HISTORYPRESENTER_H_FAVPGZSN */
