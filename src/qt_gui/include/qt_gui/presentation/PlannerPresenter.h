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
#ifndef PLANNERPRESENTER_H_FPLV5C7S
#define PLANNERPRESENTER_H_FPLV5C7S

#include "api/com_query/QueryHandler.h"
#include "api/dtos/TaskTreeDTO.h"
#include "api/requests/ReadTaskTreeQuery.h"
#include "qt_gui/presentation/PlannerContract.h"

namespace sprint_timer::ui {

class PlannerPresenter : public contracts::PlannerContract::Presenter {
public:
    using read_planner_handler_t = asp::QueryHandler<api::ReadTaskTreeQuery>;

    explicit PlannerPresenter(read_planner_handler_t& readPlannerHandler_)
        : readPlannerHandler{readPlannerHandler_}
    {
    }

private:
    read_planner_handler_t& readPlannerHandler;
    api::TaskTreeDTO data;

    void updateViewImpl() override
    {
        if (auto v = view(); v) {
            v.value()->displayPlanner(data);
        }
    }

    void fetchDataImpl() override
    {
        data = readPlannerHandler.handle(api::ReadTaskTreeQuery{});
    }
};

} // namespace sprint_timer::ui

#endif /* end of include guard: PLANNERPRESENTER_H_FPLV5C7S */
