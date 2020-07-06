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
#ifndef MANUALSPRINTADDPRESENTER_H_LEM6A0J3
#define MANUALSPRINTADDPRESENTER_H_LEM6A0J3

#include "qt_gui/presentation/ManualSprintAddContract.h"
#include <core/CommandHandler.h>
#include <core/QueryHandler.h>
#include <core/SprintBuilder.h>
#include <core/use_cases/register_sprint/RegisterSprintBulkCommand.h>
#include <core/use_cases/request_tasks/UnfinishedTasksQuery.h>

namespace sprint_timer::ui {

class ManualSprintAddPresenter
    : public contracts::ManualSprintAddContract::Presenter {
public:
    ManualSprintAddPresenter(
        CommandHandler<use_cases::RegisterSprintBulkCommand>&
            registerSprintsHandler,
        QueryHandler<use_cases::UnfinishedTasksQuery,
                     std::vector<entities::Task>>& activeTasksHandler,
        dw::Weekday firstDayOfWeek,
        std::chrono::minutes sprintDuration);

    void onSprintAddRequested() override;

    void onSprintAddConfirmed(size_t taskNumber,
                              dw::DateTime firstSprintStart,
                              size_t numSprints) override;

    void updateViewImpl() override;

private:
    CommandHandler<use_cases::RegisterSprintBulkCommand>&
        registerSprintsHandler;
    QueryHandler<use_cases::UnfinishedTasksQuery, std::vector<entities::Task>>&
        activeTasksHandler;
    dw::Weekday firstDayOfWeek;
    std::chrono::minutes sprintDuration;
};

} // namespace sprint_timer::ui

#endif /* end of include guard: MANUALSPRINTADDPRESENTER_H_LEM6A0J3 */
