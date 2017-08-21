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
#ifndef EDITTASKCOMMAND_H_3FYYCQWP
#define EDITTASKCOMMAND_H_3FYYCQWP

#include "core/ITaskStorageWriter.h"
#include "core/Command.h"

namespace core::use_cases {

class EditTask : public Command {
public:
    EditTask(ITaskStorageWriter& writer,
             Task originalTask,
             Task editedTask);

    void execute() final;

    void undo() final;

    std::string describe() const final;

private:
    ITaskStorageWriter& writer;
    Task originalTask_;
    Task editedTask_;
};

} // namespace core::use_cases

#endif /* end of include guard: EDITTASKCOMMAND_H_3FYYCQWP */
