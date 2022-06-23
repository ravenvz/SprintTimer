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
#ifndef TASKTREEMETADATAREADER_H_PZUHJLWM
#define TASKTREEMETADATAREADER_H_PZUHJLWM

#include "core/TaskMetadataTree.h"

namespace sprint_timer {

class TaskTreeMetadataReader {
public:
    virtual ~TaskTreeMetadataReader() = default;

    [[nodiscard]] virtual TaskMetadataTree readTree() const = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: TASKTREEMETADATAREADER_H_PZUHJLWM */
