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

#include "api/com_query/CommandHandler.h"
#include "api/com_query/QueryHandler.h"
#include "api/dtos/SprintMapper.h"
#include "api/requests/DeleteSprintCommand.h"
#include "api/requests/RequestSprintsQuery.h"
#include "api/requests/ActiveTasksQuery.h"
#include "qt_gui/presentation/TodaySprints.h"
#include <optional>

namespace sprint_timer::ui {

class TodaySprintsPresenter : public contracts::TodaySprints::Presenter {
public:
    using request_sprints_hdl_t = asp::QueryHandler<api::RequestSprintsQuery>;
    using delete_sprints_hdl_t = asp::CommandHandler<api::DeleteSprintCommand>;
    TodaySprintsPresenter(delete_sprints_hdl_t& deleteSprintHandler_,
                          request_sprints_hdl_t& requestSprintsHandler_);

    void onSprintDelete(const std::string& uuid) override;

private:
    delete_sprints_hdl_t& deleteSprintHandler;
    request_sprints_hdl_t& requestSprintsHandler;
    std::optional<api::RequestSprintsQuery::Result> data;

    void fetchDataImpl() override;

    void updateViewImpl() override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: TODAYSPRINTSPRESENTER_H_8U0DLSNR */

