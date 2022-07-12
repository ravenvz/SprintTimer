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
#ifndef CHANGEWORKSCHEDULEHANDLER_H_RVXXWNZ0
#define CHANGEWORKSCHEDULEHANDLER_H_RVXXWNZ0

#include "api/com_query/CommandHandler.h"
#include "api/requests/ChangeWorkScheduleCommand.h"
#include "core/ActionInvoker.h"
#include "core/WorkScheduleWriter.h"

namespace sprint_timer::api {

class ChangeWorkScheduleHandler
    : public asp::CommandHandler<ChangeWorkScheduleCommand> {
public:
    ChangeWorkScheduleHandler(WorkScheduleWriter& writer,
                              ActionInvoker& actionInvoker);

    void handle(const ChangeWorkScheduleCommand& command) override;

private:
    WorkScheduleWriter& writer;
    ActionInvoker& actionInvoker;
};

} // namespace sprint_timer::api

#endif /* end of include guard: CHANGEWORKSCHEDULEHANDLER_H_RVXXWNZ0 */
