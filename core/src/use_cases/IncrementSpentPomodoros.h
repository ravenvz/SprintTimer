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
#ifndef INCREMENTSPENTPOMODOROS_H_2OKTXAHD
#define INCREMENTSPENTPOMODOROS_H_2OKTXAHD

#include "core/ITaskStorageWriter.h"
#include "core/RevertableCommand.h"

namespace UseCases {
class IncrementSpentPomodoros : public RevertableCommand {
public:
    IncrementSpentPomodoros(ITaskStorageWriter& taskStorageWriter,
                            const std::string& taskUuid)
        : writer{taskStorageWriter}
        , taskUuid{taskUuid}
    {
    }

    std::string inspect() const final
    {
        return "Increment spent pomodoros for " + taskUuid;
    }

protected:
    void executeAction() final { writer.incrementSpentPomodoros(taskUuid); }

    void undoAction() final { writer.decrementSpentPomodoros(taskUuid); }

private:
    ITaskStorageWriter& writer;
    const std::string taskUuid;
};

} /* UseCases */

#endif /* end of include guard: INCREMENTSPENTPOMODOROS_H_2OKTXAHD */
