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
#ifndef EDITTASKCOMMAND_H_3FYYCQWP
#define EDITTASKCOMMAND_H_3FYYCQWP

#include "core/Action.h"
#include "core/TaskStorageWriter.h"

namespace sprint_timer::actions {

class EditTask : public Action {
public:
    EditTask(TaskStorageWriter& writer,
             entities::Task originalTask,
             const entities::Task& editedTask);

    void execute() final;

    void undo() final;

    std::string describe() const final;

private:
    TaskStorageWriter& writer;
    const entities::Task editedTask;
    const entities::Task originalTask;
};

} // namespace sprint_timer::actions

#endif /* end of include guard: EDITTASKCOMMAND_H_3FYYCQWP */
