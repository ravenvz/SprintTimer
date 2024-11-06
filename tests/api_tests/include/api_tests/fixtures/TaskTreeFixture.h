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
#ifndef TASKTREEFIXTURE_H_S5Q2CJOR
#define TASKTREEFIXTURE_H_S5Q2CJOR

#include "api/com_query/CommandHandler.h"
#include "api/dtos/TaskTreeDTO.h"
#include "api/requests/CreateTaskCommand.h"
#include "api/requests/RegisterSprintBulkCommand.h"
#include "api_tests/QtStorageInitializer.h"

namespace fixtures {

auto givenSomeTaskTreeCreated(
    asp::CommandHandler<sprint_timer::api::CreateTaskCommand>&
        createTaskHandler,
    asp::CommandHandler<sprint_timer::api::RegisterSprintBulkCommand>&
        registerSprintsHandler,
    TestStorageInitializer& initializer) -> sprint_timer::api::TaskTreeDTO;

auto givenTaskTreeWithDueDatesCreated(
    asp::CommandHandler<sprint_timer::api::CreateTaskCommand>&
        createTaskHandler,
    asp::CommandHandler<sprint_timer::api::RegisterSprintBulkCommand>&
        registerSprintsHandle,
    TestStorageInitializer& initializerr) -> sprint_timer::api::TaskTreeDTO;

auto givenTaskTreeWithRecurringTasksCreated(
    asp::CommandHandler<sprint_timer::api::CreateTaskCommand>&
        createTaskHandler,
    asp::CommandHandler<sprint_timer::api::RegisterSprintBulkCommand>&
        registerSprintsHandler,
    TestStorageInitializer& initializer) -> sprint_timer::api::TaskTreeDTO;

} // namespace fixtures

#endif /* end of include guard: TASKTREEFIXTURE_H_S5Q2CJOR */
