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
#ifndef EDITTAG_H_6NGOS1Q2
#define EDITTAG_H_6NGOS1Q2

#include "core/ITaskStorageWriter.h"
#include "core/RevertableCommand.h"
#include <iostream>

namespace UseCases {

class EditTag : public RevertableCommand {
public:
    EditTag(ITaskStorageWriter& tagStorageWriter,
            const std::string& oldName,
            const std::string& newName)
        : writer{tagStorageWriter}
        , oldName{oldName}
        , newName{newName}
    {
    }

    std::string inspect() const final
    {
        return "Edit tag (" + oldName + " -> " + newName + ")";
    }

protected:
    void executeAction() final { writer.editTag(oldName, newName); }

    void undoAction() final { writer.editTag(newName, oldName); }

private:
    ITaskStorageWriter& writer;
    const std::string oldName;
    const std::string newName;
};

} /* UseCases */

#endif /* end of include guard: EDITTAG_H_6NGOS1Q2 */
