/********************************************************************************
**
** Copyright (C) 2016, 2017 Pavel Pavlov.
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
#ifndef ADDTASKTRANSACTION_H_LEMDIQ17
#define ADDTASKTRANSACTION_H_LEMDIQ17

#include "core/ITaskStorageWriter.h"
#include "core/RevertableCommand.h"
#include "core/entities/Task.h"

#include <core/Command.h>

namespace core::use_cases {

class AddNewTask : public Command {
public:
    AddNewTask(ITaskStorageWriter& taskStorageWriter, Task task);

    void execute() final;

    void undo() final;

    std::string describe() const final;

private:
    ITaskStorageWriter& writer;
    const Task task;
};

} // namespace core::use_cases

namespace UseCases {

class AddNewTask : public RevertableCommand {
public:
    AddNewTask(ITaskStorageWriter& taskStorageWriter, const Task& task);

    std::string inspect() const final;

protected:
    void executeAction() final;

    void undoAction() final;

private:
    ITaskStorageWriter& writer;
    const Task task;
};


} /* UseCases */


#endif /* end of include guard: ADDTASKTRANSACTION_H_LEMDIQ17 */
