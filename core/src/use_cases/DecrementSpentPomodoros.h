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
#ifndef DECREMENTSPENTPOMODOROS_H_GYPTACBX
#define DECREMENTSPENTPOMODOROS_H_GYPTACBX

#include "core/ITaskStorageWriter.h"

namespace UseCases {


class DecrementSpentPomodoros : public RevertableCommand {
public:
    DecrementSpentPomodoros(ITaskStorageWriter& taskStorageWriter,
                            const std::string& taskUuid)
        : writer{taskStorageWriter}
        , taskUuid{taskUuid}
    {
    }

    void executeAction() final { writer.decrementSpentPomodoros(taskUuid); }

    void undoAction() final { writer.incrementSpentPomodoros(taskUuid); }

    std::string inspect() const final
    {
        return "Decrement spent pomodoros for " + taskUuid;
    }

private:
    ITaskStorageWriter& writer;
    const std::string taskUuid;
};

} /* UseCases */

#endif /* end of include guard: DECREMENTSPENTPOMODOROS_H_GYPTACBX */
