/********************************************************************************
**
** Copyright (C) 2016 Pavel Pavlov.
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
#ifndef REMOVESPRINTTRANSACTION_H_GAR8SZLM
#define REMOVESPRINTTRANSACTION_H_GAR8SZLM

#include "core/ISprintStorageWriter.h"
#include "core/RevertableCommand.h"

namespace UseCases {

class RemoveSprintTransaction : public RevertableCommand {
public:
    RemoveSprintTransaction(ISprintStorageWriter& writer, const Sprint& sprint);

    std::string inspect() const final;

protected:
    void executeAction() final;

    void undoAction() final;

private:
    ISprintStorageWriter& writer;
    const Sprint sprint;
};

} // namespace UseCases

#endif /* end of include guard: REMOVESPRINTTRANSACTION_H_GAR8SZLM */
