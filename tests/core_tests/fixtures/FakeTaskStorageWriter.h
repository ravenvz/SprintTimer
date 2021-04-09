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
#ifndef FAKETASKSTORAGEWRITER_H_L9TXWRHX
#define FAKETASKSTORAGEWRITER_H_L9TXWRHX

#include "FakeStorage.h"
#include "core/TaskStorageWriter.h"

class FakeTaskStorageWriter : public sprint_timer::TaskStorageWriter {
public:
    FakeTaskStorageWriter(FakeStorage<sprint_timer::entities::Task>& storage)
        : storage{storage}
    {
    }

    void save(const sprint_timer::entities::Task& task) final
    {
        storage.store(task);
    }

    void remove(const sprint_timer::entities::Task& task) final
    {
        storage.remove(task.uuid());
    }

    void edit(const sprint_timer::entities::Task& task,
              const sprint_timer::entities::Task& editedTask) final
    {
        storage.itemRef(task.uuid()) = editedTask;
    }

    void incrementSprints(const std::string& uuid) final
    {
        sprint_timer::entities::Task& item = storage.itemRef(uuid);
        item.setActualCost(item.actualCost() + 1);
    }

    void decrementSprints(const std::string& uuid) final
    {
        sprint_timer::entities::Task& item = storage.itemRef(uuid);
        item.setActualCost(item.actualCost() - 1);
    }

    void toggleTaskCompletionStatus(const std::string& uuid,
                                    const dw::DateTime& timeStamp) final
    {
        sprint_timer::entities::Task& taskRef = storage.itemRef(uuid);
        taskRef.setCompleted(!taskRef.isCompleted());
        taskRef.setModifiedTimeStamp(timeStamp);
    }

    void updatePriorities(const std::vector<std::string>& priorities) {}

    void editTag(const std::string& oldName, const std::string& newName)
    {
        storage.editTag(oldName, newName);
    }

    FakeStorage<sprint_timer::entities::Task>& storage;
};

#endif /* end of include guard: FAKETASKSTORAGEWRITER_H_L9TXWRHX */
