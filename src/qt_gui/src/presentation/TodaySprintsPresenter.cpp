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
#include "qt_gui/presentation/TodaySprintsPresenter.h"

namespace sprint_timer::ui {

TodaySprintsPresenter::TodaySprintsPresenter(
    CommandHandler<use_cases::DeleteSprintCommand>& deleteSprintHandler_,
    QueryHandler<use_cases::RequestSprintsQuery, std::vector<entities::Sprint>>&
        requestSprintsHandler_)
    : deleteSprintHandler{deleteSprintHandler_}
    , requestSprintsHandler{requestSprintsHandler_}
{
}

void TodaySprintsPresenter::onSprintDelete(const SprintDTO& sprint)
{
    deleteSprintHandler.handle(use_cases::DeleteSprintCommand{fromDTO(sprint)});
}

void TodaySprintsPresenter::updateViewImpl()
{
    if (auto v = view(); v) {
        const dw::DateRange range{dw::current_date_local(),
                                  dw::current_date_local()};
        const auto sprints =
            requestSprintsHandler.handle(use_cases::RequestSprintsQuery{range});
        v.value()->displaySprints(makeDTOs(sprints));
    }
}

void TodaySprintsPresenter::onViewAttached() { updateView(); }

} // namespace sprint_timer::ui
