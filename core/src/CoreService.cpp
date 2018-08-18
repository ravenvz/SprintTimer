/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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
#include "core/CoreService.h"
#include "core/use_cases/AddNewTask.h"
#include "core/use_cases/DeleteTask.h"
#include "core/use_cases/EditTask.h"
#include "core/use_cases/RegisterNewSprint.h"
#include "core/use_cases/RemoveSprintTransaction.h"
#include "core/use_cases/RenameTag.h"
#include "core/use_cases/RequestAllTags.h"
#include "core/use_cases/RequestFinishedTasks.h"
#include "core/use_cases/RequestMinMaxYear.h"
#include "core/use_cases/RequestSprintDistribution.h"
#include "core/use_cases/RequestSprints.h"
#include "core/use_cases/RequestSprintsForTask.h"
#include "core/use_cases/RequestTasks.h"
#include "core/use_cases/RequestUnfinishedTasks.h"
#include "core/use_cases/StoreUnfinishedTasksOrder.h"
#include "core/use_cases/ToggleTaskCompletionStatus.h"

namespace sprint_timer {

using dw::DateTime;
using dw::TimeSpan;
using namespace use_cases;
using namespace entities;

CoreService::CoreService(CommandInvoker& invoker)
    : invoker{invoker}
{
}

std::string CoreService::lastCommandDescription() const
{
    return invoker.lastCommandDescription();
}

uint64_t CoreService::numRevertableCommands() const
{
    return invoker.stackSize();
}

void CoreService::undoLast() { invoker.undo(); }

void CoreService::registerUndoObserver(Observer& observer)
{
    invoker.attach(observer);
}

} // namespace sprint_timer
