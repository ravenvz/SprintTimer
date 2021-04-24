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

namespace sprint_timer::ui {

class TodaySprintsPresenter : public contracts::TodaySprints::Presenter {
public:
    TodaySprintsPresenter(
        CommandHandler<use_cases::DeleteSprintCommand>& deleteSprintHandler_,
        QueryHandler<use_cases::RequestSprintsQuery>& requestSprintsHandler_);

    void onSprintDelete(const std::string& uuid) override;

    void updateViewImpl() override;

    void onViewAttached() override;

private:
    CommandHandler<use_cases::DeleteSprintCommand>& deleteSprintHandler;
    QueryHandler<use_cases::RequestSprintsQuery>& requestSprintsHandler;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: TODAYSPRINTSPRESENTER_H_8U0DLSNR */

