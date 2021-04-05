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
#ifndef HISTORYPRESENTER_H_FAVPGZSN
#define HISTORYPRESENTER_H_FAVPGZSN

#include "qt_gui/presentation/HistoryContract.h"
#include "qt_gui/presentation/HistoryMediator.h"
#include <core/QueryHandler.h>
#include <core/use_cases/request_sprints/RequestSprintsQuery.h>
#include <core/use_cases/request_tasks/FinishedTasksQuery.h>

namespace sprint_timer::ui {

class HistoryPresenter : public contracts::HistoryContract::Presenter,
                         public HistoryColleague {
public:
    using SprintsRequestHandler = QueryHandler<use_cases::RequestSprintsQuery,
                                               std::vector<entities::Sprint>>;
    using TaskRequestHandler = QueryHandler<use_cases::FinishedTasksQuery,
                                            std::vector<entities::Task>>&;

    HistoryPresenter(SprintsRequestHandler& requestSprintsHandler,
                     TaskRequestHandler& requestTasksHandler,
                     HistoryMediator& mediator);

    ~HistoryPresenter() override;

    void onEditTaskMenuSelected(const std::string& uuid) const override;

    void onEditSprintMenuSelected(const std::string& uuid) const override;

    void onDisplayedTabChanged(int tabNumber) override;

    void onSharedDataChanged() override;

private:
    SprintsRequestHandler& requestSprintsHandler;
    TaskRequestHandler& requestTasksHandler;
    HistoryMediator& mediator;

    void updateViewImpl() override;

    void onViewAttached() override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: HISTORYPRESENTER_H_FAVPGZSN */
