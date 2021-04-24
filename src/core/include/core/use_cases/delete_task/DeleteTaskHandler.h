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
#ifndef DELETETASKHANDLER_H_JZALYBVI
#define DELETETASKHANDLER_H_JZALYBVI

#include "core/ActionInvoker.h"
#include "core/CommandHandler.h"
#include "core/SprintStorage.h"
#include "core/TaskStorage.h"
#include "core/use_cases/delete_task/DeleteTaskCommand.h"

namespace sprint_timer::use_cases {

class DeleteTaskHandler : public CommandHandler<DeleteTaskCommand> {
public:
    DeleteTaskHandler(SprintStorage& sprintStorage,
                      TaskStorage& taskStorage,
                      ActionInvoker& actionInvoker);

    void handle(DeleteTaskCommand&& command) override;

private:
    SprintStorage& sprintStorage;
    TaskStorage& taskStorage;
    ActionInvoker& actionInvoker;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: DELETETASKHANDLER_H_JZALYBVI */
