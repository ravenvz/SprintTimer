/********************************************************************************
**
** Copyright (C) 2016-2018 Pavel Pavlov.
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

#include "core/Command.h"
#include "core/ISprintStorageWriter.h"

namespace sprint_timer::use_cases {

class RemoveSprintTransaction : public Command {
public:
    RemoveSprintTransaction(ISprintStorageWriter& writer,
                            entities::Sprint sprintToRemove);

    void execute() final;

    void undo() final;

    std::string describe() const final;

private:
    ISprintStorageWriter& writer;
    const entities::Sprint sprint;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: REMOVESPRINTTRANSACTION_H_GAR8SZLM */
