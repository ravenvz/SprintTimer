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
#ifndef CANCELTIMERHANDLER_H_FAY26HRG
#define CANCELTIMERHANDLER_H_FAY26HRG

#include "core/CommandHandler.h"
#include "core/IWorkflow.h"
#include "core/use_cases/workflow_control/CancelTimer.h"

namespace sprint_timer::use_cases {

class CancelTimerHandler : public CommandHandler<CancelTimer> {
public:
    explicit CancelTimerHandler(IWorkflow& workflow);

    void handle(CancelTimer&& command) override;

private:
    IWorkflow& workflow;
};

inline CancelTimerHandler::CancelTimerHandler(IWorkflow& workflow_)
    : workflow{workflow_}
{
}

inline void CancelTimerHandler::handle(CancelTimer&&) { workflow.cancel(); }

} // namespace sprint_timer::use_cases

#endif /* end of include guard: CANCELTIMERHANDLER_H_FAY26HRG */
