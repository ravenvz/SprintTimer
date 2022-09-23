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
#ifndef SAVETASKTREEHANDLER_H_K84IJWNM
#define SAVETASKTREEHANDLER_H_K84IJWNM

#include "api/ActionInvoker.h"
#include "api/TaskStorage.h"
#include "api/com_query/CommandHandler.h"
#include "api/requests/SaveTaskTreeCommand.h"
#include "core/utils/Converter.h"

namespace sprint_timer::api {

class SaveTaskTreeHandler : public asp::CommandHandler<SaveTaskTreeCommand> {
public:
    SaveTaskTreeHandler(TaskStorage& taskStorage_,
                        ActionInvoker& actionInvoker_,
                        const Converter<TaskTreeDTO, TaskTree>& taskTreeMapper);

    void handle(const SaveTaskTreeCommand& command) override;

private:
    TaskStorage& taskStorage;
    ActionInvoker& actionInvoker;
    const Converter<TaskTreeDTO, TaskTree>& taskTreeMapper;
};

} // namespace sprint_timer::api

#endif /* end of include guard: SAVETASKTREEHANDLER_H_K84IJWNM */
