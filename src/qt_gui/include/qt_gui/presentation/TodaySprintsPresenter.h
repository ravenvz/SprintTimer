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
#ifndef TODAYSPRINTSPRESENTER_H_8U0DLSNR
#define TODAYSPRINTSPRESENTER_H_8U0DLSNR

#include "qt_gui/presentation/SprintMapper.h"
#include "qt_gui/presentation/TodaySprints.h"
#include <core/CommandHandler.h>
#include <core/QueryHandler.h>
#include <core/use_cases/delete_sprint/DeleteSprintCommand.h>
#include <core/use_cases/request_sprints/RequestSprintsQuery.h>
#include <core/use_cases/request_tasks/UnfinishedTasksQuery.h>

namespace sprint_timer::ui {

class TodaySprintsPresenter : public contracts::TodaySprints::Presenter {
public:
    TodaySprintsPresenter(
        CommandHandler<use_cases::DeleteSprintCommand>& deleteSprintHandler_,
        QueryHandler<use_cases::RequestSprintsQuery,
                     std::vector<entities::Sprint>>& requestSprintsHandler_)
        : deleteSprintHandler{deleteSprintHandler_}
        , requestSprintsHandler{requestSprintsHandler_}
    {
    }

    void onSprintDelete(const SprintDTO& sprint) override
    {
        deleteSprintHandler.handle(
            use_cases::DeleteSprintCommand{fromDTO(sprint)});
    }

    void updateViewImpl() override
    {
        if (auto v = view(); v) {
            const dw::DateRange range{dw::current_date_local(),
                                      dw::current_date_local()};
            const auto sprints = requestSprintsHandler.handle(
                use_cases::RequestSprintsQuery{range});
            v.value()->displaySprints(makeDTOs(sprints));
        }
    }

    void onViewAttached() override { updateView(); }

private:
    CommandHandler<use_cases::DeleteSprintCommand>& deleteSprintHandler;
    QueryHandler<use_cases::RequestSprintsQuery, std::vector<entities::Sprint>>&
        requestSprintsHandler;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: TODAYSPRINTSPRESENTER_H_8U0DLSNR */

