/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
**
**
** This file is part of PROG_NAME.
**
** PROG_NAME is free software: you can redistribute it and/or modify
** it under the terms of the GNU Lesser General Public License as published by
** the Free Software Foundation, either version 3 of the License, or
** (at your option) any later version.
**
** PROG_NAME is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU Lesser General Public License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with PROG_NAME.  If not, see <http://www.gnu.org/licenses/>.
**
*********************************************************************************/
#ifndef EDITTASKCOMMAND_H_3FYYCQWP
#define EDITTASKCOMMAND_H_3FYYCQWP

#include "core/ITaskStorageWriter.h"
#include "core/RevertableCommand.h"

namespace UseCases {

class EditTask : public RevertableCommand {
public:
    EditTask(ITaskStorageWriter& writer,
             const Task& task,
             const Task& editedTask)
        : writer{writer}
        , task{task}
        , editedTask{editedTask.name(),
                     editedTask.estimatedCost(),
                     task.actualCost(),
                     task.uuid(),
                     editedTask.tags(),
                     task.isCompleted(),
                     DateTime::currentDateTimeLocal()}
    {
    }

    void executeAction() final { writer.edit(task, editedTask); }

    void undoAction() final { writer.edit(editedTask, task); }

    std::string inspect() const final
    {
        return "Edit task '" + task.toString() + " -> " + editedTask.toString()
            + "'";
    }


private:
    ITaskStorageWriter& writer;
    Task task;
    Task editedTask;
};

} /* UseCases */

#endif /* end of include guard: EDITTASKCOMMAND_H_3FYYCQWP */
