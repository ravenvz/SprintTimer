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
#ifndef FAKETASKSTORAGEWRITER_H_L9TXWRHX
#define FAKETASKSTORAGEWRITER_H_L9TXWRHX

#include "FakeStorage.h"
#include "core/ITaskStorageWriter.h"

class FakeTaskStorageWriter : public ITaskStorageWriter {
public:
    FakeTaskStorageWriter(FakeStorage<Task>& storage)
        : storage{storage}
    {
    }

    void save(const Task& task) final { storage.store(task); }

    void remove(const Task& task) final { storage.remove(task.uuid()); }

    void edit(const Task& task, const Task& editedTask) final
    {
        storage.itemRef(task.uuid()) = editedTask;
    }

    void incrementSpentPomodoros(const std::string& uuid) final
    {
        Task& item = storage.itemRef(uuid);
        item.setSpentPomodoros(item.spentPomodoros() + 1);
    }

    void decrementSpentPomodoros(const std::string& uuid) final
    {
        Task& item = storage.itemRef(uuid);
        item.setSpentPomodoros(item.spentPomodoros() - 1);
    }

    void toggleTaskCompletionStatus(const std::string& uuid,
                                    const DateTime& timeStamp) final
    {
        Task& taskRef = storage.itemRef(uuid);
        taskRef.setCompleted(!taskRef.isCompleted());
        taskRef.setModifiedTimeStamp(timeStamp);
    }

    void updatePriorities(std::vector<std::pair<std::string, int>>&& priorities)
    {
    }

    void editTag(const std::string& oldName, const std::string& newName)
    {
        storage.editTag(oldName, newName);
    }

    FakeStorage<Task>& storage;
};

#endif /* end of include guard: FAKETASKSTORAGEWRITER_H_L9TXWRHX */
