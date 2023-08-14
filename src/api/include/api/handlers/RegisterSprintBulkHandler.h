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
#include "api/ActionInvoker.h"
#include "api/SprintStorage.h"
#include "api/TaskStorageReader.h"
#include "api/com_query/CommandHandler.h"
#include "api/requests/RegisterSprintBulkCommand.h"
#include "cpp_utils/patterns/Converter.h"

namespace sprint_timer::api {

class RegisterSprintBulkHandler
    : public asp::CommandHandler<RegisterSprintBulkCommand> {
public:
    RegisterSprintBulkHandler(
        TaskStorageReader& taskReader,
        SprintStorage& sprintStorage,
        ActionInvoker& actionInvoker,
        const patterns::Converter<dw::DateTimeRange, Sprint>& sprintMapper);

    void handle(const RegisterSprintBulkCommand& command) override;

private:
    TaskStorageReader& taskReader;
    SprintStorage& sprintStorage;
    ActionInvoker& actionInvoker;
    const patterns::Converter<dw::DateTimeRange, Sprint>& sprintMapper;

    void throwIfTaskDoesNotExist(const std::string& taskUuid);
};

} // namespace sprint_timer::api

