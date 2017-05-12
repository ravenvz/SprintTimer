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
#ifndef TOGGLETASKCOMPLETEDCOMMAND_H_CAT08NRT
#define TOGGLETASKCOMPLETEDCOMMAND_H_CAT08NRT

#include "core/ITaskStorageWriter.h"
#include "core/RevertableCommand.h"

namespace UseCases {

class ToggleTaskCompletionStatus : public RevertableCommand {
public:
    ToggleTaskCompletionStatus(ITaskStorageWriter& taskStorageWriter,
                               const Task& task);

    std::string inspect() const final;

protected:
    void executeAction() final;

    void undoAction() final;

private:
    ITaskStorageWriter& writer;
    const std::string uuid;
    const DateTime oldTimeStamp;
};

} /* UseCases */

#endif /* end of include guard: TOGGLETASKCOMPLETEDCOMMAND_H_CAT08NRT */
