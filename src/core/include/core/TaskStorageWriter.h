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

#include "core/entities/Task.h"

namespace sprint_timer {

class TaskStorageWriter {
public:
    virtual ~TaskStorageWriter() = default;

    virtual void save(const entities::Task& task) = 0;

    virtual void remove(const std::string& uuid) = 0;

    virtual void edit(const entities::Task& oldTask,
                      const entities::Task& editedTask) = 0;

    virtual void toggleCompleted(const std::string& uuid,
                                 const dw::DateTime& timeStamp) = 0;

    virtual void
    updatePriorities(const std::vector<std::string>& priorities) = 0;

    virtual void editTag(const std::string& oldName,
                         const std::string& newName) = 0;
};

} // namespace sprint_timer

#endif /* end of include guard: ITASKSTORAGEWRITER_H_PVAMCJ6G */
