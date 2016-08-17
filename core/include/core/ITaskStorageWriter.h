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
#ifndef ITASKSTORAGEWRITER_H_PVAMCJ6G
#define ITASKSTORAGEWRITER_H_PVAMCJ6G

#include "core/entities/Task.h"

class ITaskStorageWriter {
public:
    virtual ~ITaskStorageWriter() = default;

    virtual void save(const Task& task) = 0;

    virtual void remove(const Task& task) = 0;

    virtual void edit(const Task& task, const Task& editedTask) = 0;

    virtual void incrementSprints(const std::string& uuid) = 0;

    virtual void decrementSprints(const std::string& uuid) = 0;

    virtual void toggleTaskCompletionStatus(const std::string& uuid,
                                            const DateTime& timeStamp)
        = 0;

    virtual void
    updatePriorities(std::vector<std::pair<std::string, int>>&& priorities)
        = 0;

    virtual void editTag(const std::string& oldName, const std::string& newName)
        = 0;
};

#endif /* end of include guard: ITASKSTORAGEWRITER_H_PVAMCJ6G */
