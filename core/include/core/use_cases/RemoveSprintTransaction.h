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
#ifndef REMOVESPRINTTRANSACTION_H_GAR8SZLM
#define REMOVESPRINTTRANSACTION_H_GAR8SZLM

#include "core/ISprintStorageWriter.h"
#include "core/RevertableCommand.h"

namespace UseCases {

class RemoveSprintTransaction : public RevertableCommand {
public:
    RemoveSprintTransaction(ISprintStorageWriter& writer,
                              const Sprint& sprint)
        : writer{writer}
        , sprint{sprint}
    {
    }

    void executeAction() final { writer.remove(sprint); }

    void undoAction() final { writer.save(sprint); }

    std::string inspect() const final
    {
        return "Remove sprint '" + sprint.toString() + "'";
    }

private:
    ISprintStorageWriter& writer;
    const Sprint sprint;
};

} // namespace UseCases

#endif /* end of include guard: REMOVESPRINTTRANSACTION_H_GAR8SZLM */
