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
#ifndef RENAMETAGHANDLER_H_LKXW0OMW
#define RENAMETAGHANDLER_H_LKXW0OMW

#include "core/ActionInvoker.h"
#include "core/CommandHandler.h"
#include "core/TaskStorageWriter.h"
#include "core/use_cases/rename_tag/RenameTagCommand.h"

namespace sprint_timer::use_cases {

class RenameTagHandler : public CommandHandler<RenameTagCommand> {
public:
    RenameTagHandler(TaskStorageWriter& writer, ActionInvoker& actionInvoker);

    void handle(RenameTagCommand&& command);

private:
    TaskStorageWriter& writer;
    ActionInvoker& actionInvoker;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: RENAMETAGHANDLER_H_LKXW0OMW */
