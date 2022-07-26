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
#include "api/handlers/DeleteSprintHandler.h"
#include "api/HandlerException.h"
#include "api/actions/DeleteSprint.h"

namespace sprint_timer::api {

DeleteSprintHandler::DeleteSprintHandler(SprintStorageWriter& sprintStorage_,
                                         ActionInvoker& actionInvoker_)
    : sprintStorage{sprintStorage_}
    , actionInvoker{actionInvoker_}
{
}

void DeleteSprintHandler::handle(const DeleteSprintCommand& command)
{
    // TODO remove when switched sprints
    const entities::Sprint sprint{"", command.timeRange, {}, "", ""};
    actionInvoker.execute(
        std::make_unique<actions::DeleteSprint>(sprintStorage, sprint));
}

} // namespace sprint_timer::api
