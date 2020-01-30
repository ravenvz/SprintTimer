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
#ifndef TOGGLETASKCOMPLETEDHANDLER_H_IEDFOGBK
#define TOGGLETASKCOMPLETEDHANDLER_H_IEDFOGBK

#include "core/ActionInvoker.h"
#include "core/CommandHandler.h"
#include "core/TaskStorageWriter.h"
#include "core/use_cases/toggle_task_completed/ToggleTaskCompletedCommand.h"

namespace sprint_timer::use_cases {

class ToggleTaskCompletedHandler
    : public CommandHandler<ToggleTaskCompletedCommand> {
public:
    ToggleTaskCompletedHandler(TaskStorageWriter& writer,
                               ActionInvoker& actionInvoker);

    void handle(ToggleTaskCompletedCommand&& command) override;

private:
    TaskStorageWriter& writer;
    ActionInvoker& actionInvoker;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: TOGGLETASKCOMPLETEDHANDLER_H_IEDFOGBK */
