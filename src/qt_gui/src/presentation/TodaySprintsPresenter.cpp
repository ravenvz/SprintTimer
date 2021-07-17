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
    delete_sprints_hdl_t& deleteSprintHandler_,
    request_sprints_hdl_t& requestSprintsHandler_)
    : deleteSprintHandler{deleteSprintHandler_}
    , requestSprintsHandler{requestSprintsHandler_}
{
}

void TodaySprintsPresenter::onSprintDelete(const std::string& uuid)
{
    deleteSprintHandler.handle(use_cases::DeleteSprintCommand{uuid});
}

void TodaySprintsPresenter::fetchDataImpl()
{
    const dw::DateRange range{dw::current_date_local(),
                              dw::current_date_local()};
    data = requestSprintsHandler.handle(use_cases::RequestSprintsQuery{range});
}

void TodaySprintsPresenter::updateViewImpl()
{
    if (auto v = view(); v && data) {
        v.value()->displaySprints(*data);
    }
}

} // namespace sprint_timer::ui
