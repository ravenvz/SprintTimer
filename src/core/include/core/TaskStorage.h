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
#ifndef ITASKSTORAGE_H_8CEZB2YU
#define ITASKSTORAGE_H_8CEZB2YU

#include "core/TaskStorageReader.h"
#include "core/TaskStorageWriter.h"

namespace sprint_timer {

class TaskStorage : public TaskStorageReader, public TaskStorageWriter {
};

} // namespace sprint_timer

#endif /* end of include guard: ITASKSTORAGE_H_8CEZB2YU */
