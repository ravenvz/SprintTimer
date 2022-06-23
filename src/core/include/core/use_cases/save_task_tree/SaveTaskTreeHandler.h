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

#include "core/ActionInvoker.h"
#include "core/CommandHandler.h"
#include "core/TaskTreeMetadataStorage.h"
#include "core/use_cases/save_task_tree/SaveTaskTreeCommand.h"

namespace sprint_timer::use_cases {

class SaveTaskTreeHandler : public CommandHandler<SaveTaskTreeCommand> {
public:
    SaveTaskTreeHandler(TaskTreeMetadataStorage& taskTreeStorage_,
                        ActionInvoker& actionInvoker_);

    void handle(SaveTaskTreeCommand&& command) override;

private:
    TaskTreeMetadataStorage& taskTreeStorage;
    ActionInvoker& actionInvoker;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: SAVETASKTREEHANDLER_H_K84IJWNM */
