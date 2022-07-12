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

#include "api/com_query/CommandHandler.h"
#include "api/requests/DeleteTaskCommand.h"
#include "core/ActionInvoker.h"
#include "core/SprintStorage.h"
#include "core/TaskStorage.h"

namespace sprint_timer::api {

class DeleteTaskHandler : public asp::CommandHandler<DeleteTaskCommand> {
public:
    DeleteTaskHandler(TaskStorage& taskStorage, ActionInvoker& actionInvoker);

    void handle(const DeleteTaskCommand& command) override;

private:
    TaskStorage& taskStorage;
    ActionInvoker& actionInvoker;
};

} // namespace sprint_timer::api

#endif /* end of include guard: DELETETASKHANDLER_H_JZALYBVI */
