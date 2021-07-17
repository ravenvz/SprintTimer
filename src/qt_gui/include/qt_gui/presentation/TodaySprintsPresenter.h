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

#include "core/CommandHandler.h"
#include "core/QueryHandler.h"
#include "core/use_cases/SprintMapper.h"
#include "core/use_cases/delete_sprint/DeleteSprintCommand.h"
#include "core/use_cases/request_sprints/RequestSprintsQuery.h"
#include "core/use_cases/request_tasks/ActiveTasksQuery.h"
#include "qt_gui/presentation/TodaySprints.h"
#include <optional>

namespace sprint_timer::ui {

class TodaySprintsPresenter : public contracts::TodaySprints::Presenter {
public:
    using request_sprints_hdl_t = QueryHandler<use_cases::RequestSprintsQuery>;
    using delete_sprints_hdl_t = CommandHandler<use_cases::DeleteSprintCommand>;
    TodaySprintsPresenter(delete_sprints_hdl_t& deleteSprintHandler_,
                          request_sprints_hdl_t& requestSprintsHandler_);

    void onSprintDelete(const std::string& uuid) override;

private:
    delete_sprints_hdl_t& deleteSprintHandler;
    request_sprints_hdl_t& requestSprintsHandler;
    std::optional<request_sprints_hdl_t::result_t> data;

    void fetchDataImpl() override;

    void updateViewImpl() override;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: TODAYSPRINTSPRESENTER_H_8U0DLSNR */

