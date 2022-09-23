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
#ifndef CREATETASKHANDLER_H_COE3EMLV
#define CREATETASKHANDLER_H_COE3EMLV

#include "api/ActionInvoker.h"
#include "api/DateTimeProvider.h"
#include "api/TaskStorage.h"
#include "api/UUIDGenerator.h"
#include "api/com_query/CommandHandler.h"
#include "api/dtos/TaskDTO.h"
#include "api/requests/CreateTaskCommand.h"
#include "core/utils/Converter.h"

namespace sprint_timer::api {

class CreateTaskHandler : public asp::CommandHandler<CreateTaskCommand> {
public:
    CreateTaskHandler(TaskStorage& taskStorage_,
                      ActionInvoker& actionInvoker_,
                      UUIDGenerator& uuidGenerator_,
                      DateTimeProvider& dateTimeProvider_,
                      const Converter<TaskDTO, Task>& taskMapper_);

    auto handle(const CreateTaskCommand& command) -> void override;

private:
    TaskStorage& taskStorage;
    ActionInvoker& actionInvoker;
    UUIDGenerator& uuidGenerator;
    DateTimeProvider& dateTimeProvider;
    const Converter<TaskDTO, Task>& taskMapper;
};

} // namespace sprint_timer::api

#endif /* end of include guard: CREATETASKHANDLER_H_COE3EMLV */
