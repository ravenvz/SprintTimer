/********************************************************************************
**
** Copyright (C) 2016-2019 Pavel Pavlov.
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

#include "core/Command.h"
#include "core/ITaskStorageWriter.h"

namespace sprint_timer::use_cases {

class ToggleTaskCompletionStatus : public Command {
public:
    ToggleTaskCompletionStatus(ITaskStorageWriter& taskStorageWriter,
                               const entities::Task& task);

    void execute() final;

    void undo() final;

    std::string describe() const final;

private:
    ITaskStorageWriter& writer;
    const std::string uuid;
    const dw::DateTime oldTimeStamp;
};

} // namespace sprint_timer::use_cases

#endif /* end of include guard: TOGGLETASKCOMPLETEDCOMMAND_H_CAT08NRT */
