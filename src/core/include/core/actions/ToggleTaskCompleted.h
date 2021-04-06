/********************************************************************************
**
** Copyright (C) 2016-2021 Pavel Pavlov.
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
#ifndef TOGGLETASKCOMPLETED_H_BSLZSITB
#define TOGGLETASKCOMPLETED_H_BSLZSITB

#include "core/Action.h"
#include "core/TaskStorageWriter.h"

namespace sprint_timer::actions {

class ToggleTaskCompleted : public Action {
public:
    ToggleTaskCompleted(TaskStorageWriter& taskStorageWriter,
                        const entities::Task& task);

    void execute() final;

    void undo() final;

    std::string describe() const final;

private:
    TaskStorageWriter& writer;
    const std::string uuid;
    const dw::DateTime oldTimeStamp;
};

} // namespace sprint_timer::actions

#endif /* end of include guard: TOGGLETASKCOMPLETED_H_BSLZSITB */

