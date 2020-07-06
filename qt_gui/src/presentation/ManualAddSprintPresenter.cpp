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
#include "qt_gui/presentation/ManualSprintAddPresenter.h"

namespace sprint_timer::ui {

ManualSprintAddPresenter::ManualSprintAddPresenter(
    CommandHandler<use_cases::RegisterSprintBulkCommand>&
        registerSprintsHandler_,
    QueryHandler<use_cases::UnfinishedTasksQuery, std::vector<entities::Task>>&
        activeTasksHandler_,
    dw::Weekday firstDayOfWeek_,
    std::chrono::minutes sprintDuration_)
    : registerSprintsHandler{registerSprintsHandler_}
    , activeTasksHandler{activeTasksHandler_}
    , firstDayOfWeek{firstDayOfWeek_}
    , sprintDuration{sprintDuration_}
{
}

void ManualSprintAddPresenter::onSprintAddRequested()
{
    // TODO obviously, we shouldn't query for tasks twice. Not only it is
    // uneffective, but in some (very unlikely) circumstances we might get
    // different values for two calls
    const auto activeTasks =
        activeTasksHandler.handle(use_cases::UnfinishedTasksQuery{});
    view->displayAddSprintDialog(activeTasks, firstDayOfWeek, sprintDuration);
}

void ManualSprintAddPresenter::onSprintAddConfirmed(
    size_t taskNumber, dw::DateTime firstSprintStart, size_t numSprints)
{
    const auto activeTasks =
        activeTasksHandler.handle(use_cases::UnfinishedTasksQuery{});

    const auto& task = activeTasks[taskNumber];

    std::vector<entities::Sprint> sprints;

    SprintBuilder builder;
    builder.withTaskUuid(task.uuid())
        .withName(task.name())
        .withExplicitTags(task.tags());
    const dw::DateTimeRange currentRange{firstSprintStart,
                                         firstSprintStart + sprintDuration};
    for (size_t i = 0; i < numSprints; ++i) {
        sprints.push_back(
            builder
                .withTimeSpan(dw::add_offset(currentRange, i * sprintDuration))
                .build());
    }

    registerSprintsHandler.handle(
        use_cases::RegisterSprintBulkCommand{std::move(sprints)});
}

void ManualSprintAddPresenter::updateViewImpl()
{
    const auto activeTasks =
        activeTasksHandler.handle(use_cases::UnfinishedTasksQuery{});
    view->setInteractive(!activeTasks.empty());
}

} // namespace sprint_timer::ui
