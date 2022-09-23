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
#ifndef TASKTREEMETADATASTORAGE_H_TYKA93GF
#define TASKTREEMETADATASTORAGE_H_TYKA93GF

#include "api/TaskTreeMetadataReader.h"
#include "api/TaskTreeMetadataWriter.h"
#include "core/TaskTree.h"

namespace sprint_timer {

class TaskTreeMetadataStorage : public TaskTreeMetadataReader,
                                public TaskTreeMetadataWriter { };

} // namespace sprint_timer

#endif /* end of include guard: TASKTREEMETADATASTORAGE_H_TYKA93GF */

