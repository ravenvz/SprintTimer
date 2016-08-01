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
#ifndef ADDTASKTRANSACTION_H_LEMDIQ17
#define ADDTASKTRANSACTION_H_LEMDIQ17

#include "core/ITaskStorageWriter.h"
#include "core/RevertableCommand.h"
#include "core/entities/Task.h"

namespace UseCases {

class AddNewTask : public RevertableCommand {
public:
    AddNewTask(ITaskStorageWriter& taskStorageWriter, const Task& task)
        : writer{taskStorageWriter}
        , task{task}
    {
    }

    void executeAction() final { writer.save(task); }

    void undoAction() final { writer.remove(task); }

    std::string inspect() const final
    {
        return "Add new task command '" + task.toString() + "'";
    }

private:
    ITaskStorageWriter& writer;
    const Task task;
};


} /* UseCases */


#endif /* end of include guard: ADDTASKTRANSACTION_H_LEMDIQ17 */
