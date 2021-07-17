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
#ifndef TOGGLEZONEMODEHANDLER_H_ISDKUEMO
#define TOGGLEZONEMODEHANDLER_H_ISDKUEMO

#include "core/CommandHandler.h"
#include "core/IWorkflow.h"
#include "core/use_cases/workflow_control/ToggleZoneMode.h"

namespace sprint_timer::use_cases {

class ToggleZoneModeHandler : public CommandHandler<ToggleZoneMode> {
public:
    explicit ToggleZoneModeHandler(IWorkflow& workflow);

    void handle(ToggleZoneMode&& command) override;

private:
    IWorkflow& workflow;
};

inline ToggleZoneModeHandler::ToggleZoneModeHandler(IWorkflow& workflow_)
    : workflow{workflow_}
{
}

inline void ToggleZoneModeHandler::handle(ToggleZoneMode&&)
{
    workflow.toggleInTheZoneMode();
}

} // namespace sprint_timer::use_cases

#endif /* end of include guard: TOGGLEZONEMODEHANDLER_H_ISDKUEMO */
