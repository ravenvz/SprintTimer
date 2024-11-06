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
#ifndef ITASKSTORAGEWRITER_H_PVAMCJ6G
#define ITASKSTORAGEWRITER_H_PVAMCJ6G

#include "core/Task.h"
#include "core/TaskTree.h"

namespace sprint_timer::api {

class TaskStorageWriter {
public:
    virtual ~TaskStorageWriter() = default;

    virtual auto save(const Task& task) -> void = 0;

    virtual auto remove(const std::string& uuid) -> void = 0;

    virtual auto restore(const Task& task) -> void = 0;

    virtual auto edit(const Task& oldTask, const Task& editedTask) -> void = 0;

    virtual auto toggleCompleted(const std::string& uuid,
                                 const dw::DateTime& timeStamp) -> void = 0;

    virtual auto
    updatePriorities(const std::vector<std::string>& priorities) -> void = 0;

    virtual auto editTag(const std::string& oldName,
                         const std::string& newName) -> void = 0;

    virtual auto saveTree(const TaskTree& taskTree) -> void = 0;

    virtual auto saveFullTree(const TaskTree& taskTree) -> void = 0;
};

} // namespace sprint_timer::api

#endif /* end of include guard: ITASKSTORAGEWRITER_H_PVAMCJ6G */
