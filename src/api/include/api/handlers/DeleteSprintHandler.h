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
#ifndef DELETESPRINTHANDLER_H_SE1M2VMY
#define DELETESPRINTHANDLER_H_SE1M2VMY

#include "api/com_query/CommandHandler.h"
#include "api/requests/DeleteSprintCommand.h"
#include "core/ActionInvoker.h"
#include "core/SprintStorageWriter.h"

namespace sprint_timer::api {

class DeleteSprintHandler : public asp::CommandHandler<DeleteSprintCommand> {
public:
    DeleteSprintHandler(SprintStorageWriter& sprintStorage,
                        ActionInvoker& actionInvoker);

    void handle(const DeleteSprintCommand& command) override;

private:
    SprintStorageWriter& sprintStorage;
    ActionInvoker& actionInvoker;
};

} // namespace sprint_timer::api

#endif /* end of include guard: DELETESPRINTHANDLER_H_SE1M2VMY */
