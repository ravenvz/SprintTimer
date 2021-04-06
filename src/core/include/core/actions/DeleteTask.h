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
#ifndef DELETETASK_H_07POXQ41
#define DELETETASK_H_07POXQ41

#include "core/Action.h"
#include "core/SprintStorage.h"
#include "core/TaskStorageWriter.h"

namespace sprint_timer::actions {

class DeleteTask : public Action {
public:
    DeleteTask(TaskStorageWriter& taskStorageWriter,
               SprintStorage& sprintStorage,
               entities::Task taskToRemove);

    void execute() final;

    void undo() final;

    std::string describe() const final;

private:
    TaskStorageWriter& taskWriter;
    SprintStorage& sprintStorage;
    const entities::Task task;
    std::vector<entities::Sprint> taskSprints;
};

} // namespace sprint_timer::actions

#endif /* end of include guard: DELETETASK_H_07POXQ41 */

