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
#ifndef UNDOLASTCOMMANDHANDLER_H_WELR6XYB
#define UNDOLASTCOMMANDHANDLER_H_WELR6XYB

#include "api/com_query/CommandHandler.h"
#include "api/requests/UndoLastCommand.h"
#include "core/ActionInvoker.h"

namespace sprint_timer::api {

class UndoLastCommandHandler : public asp::CommandHandler<UndoLastCommand> {
public:
    explicit UndoLastCommandHandler(ActionInvoker& actionInvoker);

    void handle(const UndoLastCommand& command) override;

private:
    ActionInvoker& actionInvoker;
};

} // namespace sprint_timer::api

#endif /* end of include guard: UNDOLASTCOMMANDHANDLER_H_WELR6XYB */
