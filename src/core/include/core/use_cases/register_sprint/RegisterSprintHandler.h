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
#ifndef REGISTERSPRINTHANDLER_H_YKTWIV1B
#define REGISTERSPRINTHANDLER_H_YKTWIV1B

#include "core/ActionInvoker.h"
#include "core/CommandHandler.h"
#include "core/SprintStorageWriter.h"
#include "core/use_cases/register_sprint/RegisterSprintCommand.h"

namespace sprint_timer::use_cases {

class RegisterSprintHandler : public CommandHandler<RegisterSprintCommand> {
public:
    RegisterSprintHandler(SprintStorageWriter& writer,
                          ActionInvoker& actionInvoker);

    void handle(RegisterSprintCommand&& command) override;

private:
    SprintStorageWriter& writer;
    ActionInvoker& actionInvoker;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: REGISTERSPRINTHANDLER_H_YKTWIV1B */
