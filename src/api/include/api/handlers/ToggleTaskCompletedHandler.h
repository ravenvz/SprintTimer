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
#ifndef TOGGLETASKCOMPLETEDHANDLER_H_IEDFOGBK
#define TOGGLETASKCOMPLETEDHANDLER_H_IEDFOGBK

#include "api/com_query/CommandHandler.h"
#include "api/requests/ToggleTaskCompletedCommand.h"
#include "core/ActionInvoker.h"
#include "api/TaskStorageWriter.h"

namespace sprint_timer::api {

class ToggleTaskCompletedHandler
    : public asp::CommandHandler<ToggleTaskCompletedCommand> {
public:
    ToggleTaskCompletedHandler(TaskStorageWriter& writer,
                               ActionInvoker& actionInvoker);

    void handle(const ToggleTaskCompletedCommand& command) override;

private:
    TaskStorageWriter& writer;
    ActionInvoker& actionInvoker;
};

} // namespace sprint_timer::api

#endif /* end of include guard: TOGGLETASKCOMPLETEDHANDLER_H_IEDFOGBK */
