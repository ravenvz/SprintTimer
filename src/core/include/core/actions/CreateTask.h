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
#ifndef CREATETASK_H_ISATC8JM
#define CREATETASK_H_ISATC8JM

#include "core/Action.h"
#include "core/TaskStorageWriter.h"
#include "core/entities/Task.h"

namespace sprint_timer::actions {

class CreateTask : public Action {
public:
    CreateTask(TaskStorageWriter& taskStorageWriter, entities::Task task);

    void execute() final;

    void undo() final;

    std::string describe() const final;

private:
    TaskStorageWriter& writer;
    const entities::Task task;
};

} // namespace sprint_timer::actions

#endif /* end of include guard: CREATETASK_H_ISATC8JM */

